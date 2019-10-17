/*!***************************************************************************************
\file       BehaviorTreeManager.cpp
\author     Charles Boudousquie
\date       9/24/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the Manager that controls any behavior tree an agent may use.
*****************************************************************************************/
#include "BehaviorTreeManager.hpp"
#include "BehaviorTreeBuilder.hpp"
#include "BehaviorTree.hpp"
#include "Nodes/Behaviors/Behavior.hpp"
#include "AgentEncapsulator.hpp"
#include "Agent.hpp"
#include "GameObject.hpp"

BehaviorTreeManager::BehaviorTreeManager()
{
    this->builder = std::make_shared<BehaviorTreeBuilder>();
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
        this->tree->Update(this->getDt());
    }
}

void BehaviorTreeManager::onInitialize()
{
}

void BehaviorTreeManager::ActivateTree(const std::string & treeName)
{
    tree = this->builder->CreateTree(treeName);
    shouldUpdate = true;
}

void BehaviorTreeManager::linkAgentComponentToTree(std::shared_ptr<Agent> agent, const std::string & treeName)
{
    if (trees.find(treeName) != trees.end())
    {
        // add agent to tree
        this->trees[treeName]->AddAgent(agent);
    }
    else
    {
        std::cout << "ERROR could not add agent to tree " << treeName << std::endl;
    }
    
}

int BehaviorTreeManager::getCurrentNodeID()
{
    if (tree != nullptr && tree->GetActiveNode() != nullptr)
    {
        auto currentNode = tree->GetActiveNode();
        return currentNode->getId();
    }

    return -1;
}

