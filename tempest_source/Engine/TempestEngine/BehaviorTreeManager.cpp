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

void BehaviorTreeManager::ReloadTrees()
{
    auto actors = getAgentGameObjects();

    // for each tree that must be reloaded
    for (auto treeName : treesToReload)
    {
        for (auto actor : actors)
        {
            auto agent = actor->getComponent<Agent>();
            if (agent->getTreeName() == treeName)
            {
                // reset agent
                agent->GetTask()->ClearHistory();
            }
        }

        // update tree map
        this->trees[treeName] = builder->CreateTree(treeName);
    }

    treesToReload.clear();
}

BehaviorTreeManager::BehaviorTreeManager()
{
    this->builder = std::make_shared<BehaviorTreeBuilder>();
    this->builder->SetManager(this);
}

const std::string & BehaviorTreeManager::name() const
{
    return getName();
}

void BehaviorTreeManager::onUpdate()
{
    if (!treesToReload.empty()) { ReloadTrees(); }

    // get all actors
    std::vector<std::shared_ptr<gameObject>> actors = this->getAgentGameObjects();

    // for every single actor
    for (auto actor : actors)
    {
        // only 1 actor and 1 tree can be active in a single threaded application
        ActorDatabase::GetInstance().AssignCurrentActor(actor);

        // get agent
        auto agent = actor->getComponent<Agent>();
        auto task = agent->GetTask();

        // get tree
        auto tree = GetTree(agent->getTreeName());
        agent->Update(this->getDt(), tree);
    }
}

void BehaviorTreeManager::onInitialize() 
{
    std::cout << "Initializing BTM";
}

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

void BehaviorTreeManager::MarkTreeAsChanged(const std::string & treeName)
{
    if (trees.find(treeName) != trees.end())
    {
        treesToReload.insert(treeName);
    }
    else
    {
        std::cout << "ERROR marking tree that does not exist\n";
    }
}

