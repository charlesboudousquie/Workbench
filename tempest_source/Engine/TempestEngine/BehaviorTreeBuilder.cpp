/*!***************************************************************************************
\file       BehaviorTreeBuilder.cpp
\author     Charles Boudousquie
\date       9/24/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief      Delegate (aka helper) object that constructs trees for the BehaviorTree Manager.
*****************************************************************************************/
#include "AgentEncapsulator.hpp"
#include "BehaviorTreeBuilder.hpp"
#include "BehaviorTree.hpp"
#include <stack>


#include <rapidjson.h>
#include "document.h"
#include "stringbuffer.h"
#include "writer.h"
#include "prettywriter.h"
#include "istreamwrapper.h"
#include "filereadstream.h"

// leaves
#include "../Nodes/Behaviors/DefaultLeaf.hpp"

// decorators
#include "../Nodes/Behaviors/Inverter.hpp"
#include "../Nodes/Behaviors/Repeater.hpp"
#include "../Nodes/Behaviors/RepeatUntilN.hpp"
#include "../Nodes/Behaviors/RepeatUntilFailure.hpp"
#include "../Nodes/Behaviors/RepeatUntilSuccess.hpp"
#include "../Nodes/Behaviors/ReturnTrue.hpp"
#include "../Nodes/Behaviors/Timer.hpp"

//#include "../../Samples/Assets/graphs/"

// composites
#include "../Nodes/Behaviors/Selector.hpp"
#include "../Nodes/Behaviors/Sequencer.hpp"

#include "AssetManager.hpp"
#include "GeneralizedFileLoader.hpp"

static std::unordered_map<std::string, std::function<BehaviorPtr()>> nodeMap;

void CreateNodes(const rapidjson::Value & l_graph, std::map<int, std::vector<int>>& nodeLinks,
    std::map<int, BehaviorPtr>& behaviors);

typedef int NodeID;

BehaviorTreeBuilder::BehaviorTreeBuilder()
{
    // leaves
    nodeMap["Inverter"] = []() { return std::make_shared< Inverter>(); };
    nodeMap["Repeater"] = []() {return std::make_shared< Repeater>(); };
    nodeMap["RepeatUntilN"] = []() {return std::make_shared< RepeatUntilN>(); };
    nodeMap["RepeatUntilFailure"] = []() {return std::make_shared< RepeatUntilFailure>(); };
    nodeMap["RepeatUntilSuccess"] = []() {return std::make_shared< RepeatUntilSuccess>(); };
    nodeMap["ReturnTrue"] = []() {return std::make_shared< ReturnTrue>(); };
    nodeMap["DefaultLeaf"] = []() {return std::make_shared<DefaultLeaf>(); };
    nodeMap["Timer"] = []() {return std::make_shared<Timer>(); };

    // composites
    nodeMap["Selector"] = []() {return std::make_shared< Selector>(); };
    nodeMap["Sequencer"] = []() {return std::make_shared< Sequencer>(); };

}

std::shared_ptr<BehaviorTree> BehaviorTreeBuilder::CreateTree(const std::string & fileName)
{
    auto assetList = assetManager::assetList(".graph");
    std::string filePath = "../../Samples/Assets/";
    // find specific item in assetlist with fileName
    for (auto asset : assetList)
    {
        if (asset.find(fileName) != std::string::npos)
        {
            filePath += asset;
        }
    }

    std::ifstream l_if_stream(filePath);

    std::map<NodeID, std::vector<NodeID>> nodeLinks;
    std::map<NodeID, BehaviorPtr> behaviors;
    std::shared_ptr<BehaviorTree> tree = std::make_shared<BehaviorTree>();

    // if file is open
    if (l_if_stream.is_open())
    {
        // turn file into string for easier reading
        std::string file_as_string((std::istreambuf_iterator<char>(l_if_stream)), std::istreambuf_iterator<char>());

        // create json document from data
        rapidjson::Document l_document;
        l_document.Parse(file_as_string.c_str());

        // if this is a graph file
        if (l_document.HasMember("graph"))
        {

            // get graph object
            const rapidjson::Value & l_graph = l_document["graph"];

            CreateNodes(l_graph, nodeLinks, behaviors);
            LinkNodes(nodeLinks, behaviors);
        }


        // give tree pointer to each node, so the nodes can talk to it
        for (auto behavior : behaviors)
        {
            behavior.second->setParentTree(tree);
        }

        tree->SetName(fileName);
        tree->SetUpTree(behaviors.begin()->second, behaviors);

    }
    else
    {
        std::cout << "ERROR tree" << fileName << "Cant be loaded";
    }
    l_if_stream.close();

    return tree;
}

// create node list from json object
void CreateNodes(const rapidjson::Value & l_graph, std::map<int, std::vector<int>>& nodeLinks,
    std::map<int, BehaviorPtr>& behaviors)
{

    // for each node in graph
    for (int i = 0; i < l_graph.Size(); i++)
    {
        // get node
        auto& node = l_graph[i];

        // if it is an actual node
        if (node.IsObject())
        {
            std::string name;
            // get node name
            if (node.HasMember("name"))
            {
                name = node["name"].GetString();
            }

            NodeID id;
            // get node id
            if (node.HasMember("id"))
            {
                id = node["id"].GetInt();
            }

            // create Behavior based on name
            BehaviorPtr behavior = nodeMap[name]();

            // set behaviors id
            behavior->setId(id);

            // store node in behavior list
            behaviors[id] = behavior;

            // if node has links
            if (node.HasMember("node links"))
            {
                // get links
                const rapidjson::Value & l_node_links = node["node links"];

                std::vector<NodeID> children;

                // get id of each child
                for (int j = 0; j < l_node_links.Size(); ++j)
                {
                    ////InputIdx, InputSlot, OutputIdx, OutputSlot;

                    // child happens to be InputIDx
                    auto child = l_node_links[j][0].GetInt();
                    children.push_back(child);
                }

                // store nodelink in nodelink list
                nodeLinks[id] = children;
            }
        }
    }
}

void BehaviorTreeBuilder::SetManager(BehaviorTreeManager * manager_)
{
    this->manager = manager_;
}
//
//std::shared_ptr<BehaviorTree> BehaviorTreeBuilder::GetTree(std::string fileName)
//{
//    auto table = BuildTable(fileName);
//
//    auto root = BuildTree(table);
//    //
//    auto tree = std::make_shared<BehaviorTree>();
//    tree->SetUpTree(root);
//
//    return tree;
//}

void BehaviorTreeBuilder::LinkNodes(std::map<int, std::vector<NodeID>>& nodeLinks, std::map<int, BehaviorPtr>& behaviors)
{
    // for every pairs in the nodelinks
    for (auto pair : nodeLinks)
    {
        //get parent node
        auto ParentID = pair.first;
        BehaviorPtr ParentNode = behaviors[ParentID];

        // get list of child ids
        auto Children = pair.second;

        // for each child, add it to node
        for (auto Child : Children)
        {
            auto ChildNode = behaviors[Child];
            ParentNode->addChild(ChildNode);
            ChildNode->setParent(ParentNode);
        }
    }
}

//BehaviorTreeBuilder::BehaviorPtr BehaviorTreeBuilder::BuildTree(const Table & table)
//{
//    std::for_each(table.begin(), table.end(),
//        [](const TableEntry& entry)
//    {
//        if (entry.behavior == nullptr)
//        {
//            throw std::exception("empty entry");
//        }});
//
//    // create tree // MAKE SURE TABLE'S POINTERS ARE CORRECT
//    BehaviorPtr tree = table.begin()->behavior;
//
//    // stack to help create tree
//    std::stack<TableEntry> stack;
//    // push on root
//    stack.push(*table.begin());
//
//    // set iterator which traverses the table,
//    // when done traversing the tree should be fully built
//    auto it = table.begin() + 1;
//
//    // iterate over each node in the list that was read from a file.
//    while (it != table.end())
//    {
//
//        auto top = stack.top();
//
//        // if lower level then it is a child
//        if (it->level > top.level)
//        {
//            // add as child to the current node
//            top.behavior->addChild(it->behavior);
//            it->behavior->setParent(top.behavior);
//            stack.push(*it);
//            it++;
//            continue;
//        }
//        // if higher level then go back up tree
//        else if (it->level <= top.level)
//        {
//            stack.pop();
//        }
//    }
//
//    return tree;
//}
//
//BehaviorTreeBuilder::Table BehaviorTreeBuilder::BuildTable(std::string fileName)
//{
//
//    std::wstring filePath(L"behavior_trees\\");
//    filePath.append(fileName.begin(), fileName.end());
//    filePath.append(L".bht");
//    
//    auto handle = assetManager::getAsset(filePath);
//
//    
//    auto data = handle->getData< genericAssetData>().getRaw(); // ??????????????
//
//    if (data.empty())
//    {
//        throw std::exception(("Failed to Load data file: " + fileName).c_str());
//    }
//
//    std::stringstream file(std::string(data.begin(), data.end()));
//
//    Table table;
//
//    bool firstLine = true;
//
//    // get line from file
//    std::string line;
//    while (std::getline(file, line))
//    {
//        if (firstLine == true)
//        {
//            firstLine = false;
//            continue;
//        }
//
//        // find first parenthesis
//        size_t leftParenthesisPos = line.find_first_of("(");
//        size_t rightParenthesisPos = line.find_last_of(")");
//        size_t commaPos = line.find(",");
//
//        // if file is not corrupted
//        if (leftParenthesisPos != std::string::npos && rightParenthesisPos != std::string::npos &&
//            commaPos != std::string::npos)
//        {
//            std::string className = line.substr(leftParenthesisPos + 1, commaPos - leftParenthesisPos - 1);
//            std::string depthString = line.substr(commaPos + 1, rightParenthesisPos - commaPos - 1);
//
//            int depthLevel = std::stoi(depthString);
//            if (nodeMap.find(className) == nodeMap.end())
//            {
//                std::string error("Could not find node of type: ");
//                error += className;
//                throw std::exception(error.c_str());
//            }
//            else
//            {
//                auto node = nodeMap[className]();
//                // add item to table
//                TableEntry newItem;
//                newItem.behavior = node;
//                newItem.level = (unsigned)depthLevel;
//                table.push_back(newItem);
//            }
//
//        }
//        else
//        {
//            std::cout << "File " << fileName << " corrupted!" << std::endl;
//        }
//    }
//
//    return table;
//}
//



