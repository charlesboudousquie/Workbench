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
#include "BehaviorTask.hpp"

#include "GameObjectGatherer.hpp"
#include "SystemManager.hpp"
#include "Nodes/Behaviors/Behavior.hpp"
#include "Agent.hpp"
#include "GameObject.hpp"

BehaviorTreePtr BehaviorTreeManager::GetTree(const std::string &  treeName)
{
    // if tree does not exist then create it
    if (trees.find(treeName) == trees.end())
    {
        trees[treeName] = builder->CreateTree(treeName);
    }

    return trees[treeName];
}

BehaviorTreeManager::BehaviorTreeManager()
{
    this->builder = std::make_shared<BehaviorTreeBuilder>();
    //this->dataBase = std::make_shared<BehaviorTreeDataBase>();
    this->builder->SetManager(this);
}

const std::string & BehaviorTreeManager::name() const
{
    return getName();
}

void BehaviorTreeManager::onUpdate()
{
    // get all actors
    std::vector<std::shared_ptr<gameObject>> actors = this->getAgentGameObjects();

    // for every single actor
    for (auto actor : actors)
    {
        // get agent
        auto agent = actor->getComponent<Agent>();
        // get tree
        auto tree = GetTree(agent->getTreeName());
        agent->Update(this->getDt(), tree);
    }
}

void BehaviorTreeManager::onInitialize() {}

std::vector<std::shared_ptr<gameObject>> BehaviorTreeManager::getAgentGameObjects()
{
    GameObjectFiltering::componentTypeSet node_pattern;
    node_pattern.setType(Agent::getType());

    std::vector<std::shared_ptr<gameObject>> actors = getSystemManager()->getGameObjectGatherer()->getList(node_pattern);

    return actors;
}

int BehaviorTreeManager::getCurrentNodeID(GameObjectPtr actor_)
{
   auto agent = actor_->getComponent<Agent>();
   auto currentBehavior = agent->GetTask()->GetCurrentBehavior();
   if (currentBehavior)
   {
       return currentBehavior->getId();
   }

   // task not active
   return -1;
}

