/*!***************************************************************************************
\file       BehaviorTreeManager.cpp
\author     Charles Boudousquie
\date       9/24/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the Manager that controls any behavior tree an agent may use.
*****************************************************************************************/
#include "BehaviorTreeManager.hpp"
#include "BehaviorTreeDataBase.hpp"
#include "BehaviorTreeBuilder.hpp"
#include "BehaviorTree.hpp"
#include "BehaviorTask.hpp"

#include "Nodes/Behaviors/Behavior.hpp"
#include "AgentEncapsulator.hpp"
#include "Agent.hpp"
#include "GameObject.hpp"

BehaviorTreeManager::BehaviorTreeManager()
{
    this->builder = std::make_shared<BehaviorTreeBuilder>();
    this->dataBase = std::make_shared<BehaviorTreeDataBase>();
    this->builder->SetManager(this);
    shouldUpdate = false;
}

const std::string & BehaviorTreeManager::name() const
{
    return getName();
}

void BehaviorTreeManager::onUpdate()
{
    if (this->shouldUpdate)
    {
        auto tasks = dataBase->GetTasks();

        //for each task
        for (auto task : tasks)
        {
            // get tree
            auto tree = task->GetTree();

            // Tell Task Proxy to use this particular task
            tree->SetTask(task);

            // current behavior tick
            task->GetCurrentBehavior()->tick(this->getDt());
        }
    }
}

void BehaviorTreeManager::onInitialize() {}

//void BehaviorTreeManager::ActivateTree()
//{
//
//}

void BehaviorTreeManager::linkAgentComponentToTree(std::shared_ptr<Agent> agent, const std::string & treeName)
{
    // if tree exists
    if (dataBase->HasTree(treeName))
    {
        this->dataBase->AssignAgentToTree(agent, treeName);
    }
    //if tree does not exist then create it
    else
    {
        BehaviorTreePtr tree = this->builder->CreateTree(treeName);

        // if tree loading succeeded
        if (tree != nullptr)
        {
            dataBase->AddTree(tree, treeName);
        }
        // otherwise it failed
        else
        {
            std::cout << "Failed to create tree: " << treeName << std::endl;
        }
    }
}

int BehaviorTreeManager::getCurrentNodeID(AgentPtr agent)
{
    return dataBase->AgentGetActiveBehavior(agent)->getId();
}

//int BehaviorTreeManager::getCurrentNodeID()
//{
//    if (tree != nullptr && tree->GetActiveNode() != nullptr)
//    {
//        auto currentNode = tree->GetActiveNode();
//        return currentNode->getId();
//    }
//
//    return -1;
//}

