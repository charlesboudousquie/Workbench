/*!***************************************************************************************
\file       BehaviorTreeDatabase.cpp
\author     Charles Boudousquie
\date       10/17/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This is the interface for the Behavior Tree Database that keeps track of agents
            and their corresponding trees. Also keeps track of tasks.
*****************************************************************************************/
#include "BehaviorTreeDataBase.hpp"
#include <algorithm>
#include "BehaviorTask.hpp"
#include "Agent.hpp"
#include "BehaviorEnums.hpp"
#include "BehaviorTree.hpp"
#include "../Nodes/Behaviors/Behavior.hpp"

void BehaviorTreeDataBase::UnassignAgentFromTree(AgentPtr agent)
{
    auto tree = GetTreeFromAgent(agent);
    agentsToTree.erase(agent); // remove agent's link to tree
    treeToAgents[tree].erase(agent); // remove tree's link to agent

    for (auto task : tasks)
    {
        if (task->GetAgent() == agent)
        {
            tasks.erase(task);
            break;
        }
    }

    //auto taskCheck = [agent](BehaviorTaskPtr task) { return task->GetAgent() == agent; };
    //
    //// make sure agent's corresponding task is deleted
    //std::remove_if(tasks.begin(), tasks.end(), taskCheck);
}

void BehaviorTreeDataBase::AssignAgentToTree(AgentPtr newAgent, const std::string& treeName)
{
    // fill in hash tables for quick access
    agentsToTree[newAgent] = GetTree(treeName);
    treeToAgents[GetTree(treeName)].insert(newAgent);

    // create a task that links the agent and the tree together
    CreateTask(newAgent, GetTree(treeName));
}

BehaviorTaskPtr BehaviorTreeDataBase::CreateTask(AgentPtr agent, BehaviorTreePtr tree)
{
    // create a task that links agent and tree
    BehaviorTaskPtr task = std::make_shared<BehaviorTask>();
    
    // set initial state of task
    task->SetAgent(agent);

    BehaviorState state(tree->GetRoot()->getId(), 0, BehaviorPhase::STARTING);
    task->Push_State(state);
    /*task->PushChildIndex(0);
    task->Push_Phase(BehaviorPhase::STARTING);*/
    
    // give the task to the root of the tree
    tree->SetTask(task);

    return task;
}

std::unordered_map<BehaviorTreeDataBase::TreeName, BehaviorTreePtr> BehaviorTreeDataBase::GetAllTrees()
{
    return trees;
}

std::set<BehaviorTaskPtr> BehaviorTreeDataBase::GetTasks()
{
    return tasks;
}

Behavior* BehaviorTreeDataBase::AgentGetActiveBehavior(AgentPtr agent)
{
    for (auto task : tasks)
    {
        if (task->GetAgent() == agent)
        {
            return task->GetCurrentBehavior();
        }
    }
    return nullptr;
}

void BehaviorTreeDataBase::AddTree(BehaviorTreePtr tree, const std::string& treeName)
{
    trees[treeName] = tree;
}

BehaviorTreePtr BehaviorTreeDataBase::GetTreeFromAgent(AgentPtr agent)
{
    // if agent is assigned to a tree
    if (agentsToTree.find(agent) != agentsToTree.end())
    {
        return agentsToTree[agent];
    }

    // agent is unassigned
    return nullptr;
}

BehaviorTreePtr BehaviorTreeDataBase::GetTree(const std::string & treeName)
{
    if (HasTree(treeName))
    {
        return this->trees[treeName];
    }
    return nullptr;
}

bool BehaviorTreeDataBase::HasTree(const std::string & treeName)
{
    return trees.find(treeName) != trees.end();
}




