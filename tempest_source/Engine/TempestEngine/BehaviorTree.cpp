/*!***************************************************************************************
\file       BehaviorTree.hpp
\author     Charles Boudousquie
\date       9/24/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the object contains Behavior Nodes.
*****************************************************************************************/
#include "BehaviorTree.hpp"
#include "Nodes/Behaviors/Behavior.hpp"
#include "AgentEncapsulator.hpp"
#include "Agent.hpp"


void BehaviorTree::SetUpTree(std::shared_ptr<Behavior> root_, std::map<int, BehaviorPtr> nodes_)
{
    

    // set root
    this->root = root_;
    // a map of a Node's id to the actual node for quick lookup
    this->nodes = nodes_;

    // should start off with root being the first active node
    this->currentNodeID = root_->getId();

    // proxy is already set in constructor
    RecursivelySetProxy(&(this->agentProxy), this->root);

}

void BehaviorTree::RecursivelySetProxy(AgentEncapsulator* proxy, std::shared_ptr<Behavior> node)
{
    // set agent proxy
    node->setAgentProxy(proxy);

    // get child nodes
    auto children = node->GetChildren();

    // if any children then we recursively set proxy for them
    for (auto child : children)
    {
        RecursivelySetProxy(proxy, child);
    }
}

void BehaviorTree::ResetTree()
{
    root->SetPhase(BehaviorPhase::STARTING);
}

BehaviorPtr BehaviorTree::GetActiveNode()
{
    return nodes[currentNodeID];
}

BehaviorTree::BehaviorTree()
{}

void BehaviorTree::SetRoot(std::shared_ptr<Behavior> root_)
{
    root = root_;
}

void BehaviorTree::Update(float dt)
{
    //get node from map
    auto currentNode = nodes[currentNodeID];

    // tick current node
    currentNode->tick(dt);

    // if current node is done
    if (currentNode->GetPhase() == BehaviorPhase::DONE)
    {
        // if root then restart tree
        if (currentNode->getParent() == nullptr)
        {
            ResetTree();
        }
        else
        {
            // if parent then make parent handle result
            currentNode->getParent()->handleResult(currentNode->getResult());

            // set current id to be parent's id
            currentNodeID = currentNode->getParent()->getId();

            // set current to parent
            currentNode = currentNode->getParent();
        }
    }
}

void BehaviorTree::SetCurrentNode(int current)
{
    // see if id even exists
    if (nodes.find(current) != nodes.end())
    {
        this->currentNodeID = current;
    }
    else
    {
        throw std::exception("Tried to set current node in tree but id did not exist");
    }
}

void BehaviorTree::AddAgent(std::shared_ptr<Agent> agent)
{
    // make sure agent knows what tree it belong too
    agent->setTreeName(this->name);
    agents.push_back(agent);
}

std::vector<std::shared_ptr<Agent>> BehaviorTree::GetAgents()
{
    return agents;
}

//void BehaviorTree::GoUpTree()
//{
//    // get parent
//    currentNode = currentNode->getParent();
//
//    // if root then just reset to root
//    if (currentNode == nullptr)
//    {
//        currentNode = root;
//    }
//
//    currentNode->SetPhase(BehaviorPhase::STARTING);
//
//}
//
//BehaviorPtr FindActiveChild(BehaviorPtr n)
//{
//    BehaviorPtr node = n;
//
//    // while it exists and is not in the middle of an operation
//    while (n && !n->progressing())
//    {
//        n = n->getCurrentChild();
//    }
//
//    return n;
//}
//
//void BehaviorTree::GoDownTree()
//{
//    //get active child
//    BehaviorPtr activeChild = FindActiveChild(currentNode);
//
//    // if it exists then set current node to that node
//    if (activeChild)
//    {
//        currentNode = activeChild;
//    }
//}