///*!***************************************************************************************
//\file       BehaviorTreeDatabase.cpp
//\author     Charles Boudousquie
//\date       10/17/2019
//\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
//\par        Project: Boomerang
//\brief      This is the interface for the Behavior Tree Database that keeps track of agents
//            and their corresponding trees. Also keeps track of tasks.
//*****************************************************************************************/
//#include "BehaviorTreeDataBase.hpp"
//#include <algorithm>
//#include "BehaviorTask.hpp"
//#include "Agent.hpp"
//#include "BehaviorEnums.hpp"
//#include "BehaviorTree.hpp"
//#include "../Nodes/Behaviors/Behavior.hpp"
//#include "GameObject.hpp"
//
//void BehaviorTreeDataBase::UnassignActorFromTree(GameObjectPtr actor)
//{
//    auto tree = GetTreeFromActor(actor);
//
//    // if actor exists
//    if (tasks.find(actor) != tasks.end())
//    {
//        // actor is no longer working with tree thus task is done
//        tasks.erase(actor);
//    }
//    else
//    {
//        std::cout << "ERROR User tried to unassign actor that does not exist in database" << std::endl;
//    }
//}
//
//void BehaviorTreeDataBase::AssignActorToTree(GameObjectPtr actor)
//{
//
//    // if actor is already assigned to tree then unassign actor and then assign actor to new tree
//    if (HasActor(actor))
//    {
//        UnassignActorFromTree(actor);
//    }
//
//    // get agent
//    auto agent = actor->getComponent<Agent>();
//    
//    // get agent's tree name
//    auto treeName = agent->getTreeName();
//
//    // if tree exists
//    if (HasTree(treeName))
//    {
//        // get the actual tree
//        auto tree = GetTree(treeName);
//
//        // create a task that links the agent and the tree together
//        auto task = CreateTask(actor, tree);
//        tasks[actor] = task;
//    }
//    else
//    {
//        std::cout << "ERROR agent had an unknown tree" << std::endl;
//    }
//}
//
//BehaviorTaskPtr BehaviorTreeDataBase::CreateTask(GameObjectPtr agent, BehaviorTreePtr tree)
//{
//    // create a task that links agent and tree
//    BehaviorTaskPtr task = std::make_shared<BehaviorTask>();
//    
//    // set initial state of task
//    task->SetActor(agent);
//
//    BehaviorState state(tree->GetRoot()->getId(), 0, BehaviorPhase::STARTING);
//    task->Push_State(state);
//    /*task->PushChildIndex(0);
//    task->Push_Phase(BehaviorPhase::STARTING);*/
//    
//    // give the task to the root of the tree
//    tree->SetTask(task);
//
//    return task;
//}
//
//std::unordered_map<BehaviorTreeDataBase::TreeName, BehaviorTreePtr> BehaviorTreeDataBase::GetAllTrees()
//{
//    return trees;
//}
//
//std::set<BehaviorTaskPtr> BehaviorTreeDataBase::GetTasks()
//{
//    return tasks;
//}
//
//Behavior* BehaviorTreeDataBase::ActorGetActiveBehavior(GameObjectPtr agent)
//{
//    for (auto task : tasks)
//    {
//        if (task->GetActor() == agent)
//        {
//            return task->GetCurrentBehavior();
//        }
//    }
//    return nullptr;
//}
//
//void BehaviorTreeDataBase::AddTree(BehaviorTreePtr tree, const std::string& treeName)
//{
//    trees[treeName] = tree;
//}
//
//BehaviorTreePtr BehaviorTreeDataBase::GetTreeFromActor(GameObjectPtr actor)
//{
//    // get treename assigned to actor
//    auto treename = actor->getComponent<Agent>()->getTreeName();
//
//    // returns tree if it exists
//    return GetTree(treename);
//}
//
//BehaviorTreePtr BehaviorTreeDataBase::GetTree(const std::string & treeName)
//{
//    if (HasTree(treeName))
//    {
//        return this->trees[treeName];
//    }
//    return nullptr;
//}
//
//bool BehaviorTreeDataBase::HasTree(const std::string & treeName)
//{
//    return trees.find(treeName) != trees.end();
//}
//
//bool BehaviorTreeDataBase::HasActor(GameObjectPtr actor)
//{
//    return tasks.find(actor) != tasks.end();
//}
//
//void BehaviorTreeDataBase::RemoveActor(GameObjectPtr actor)
//{
//    this->tasks.erase(actor);
//}
//
//
//
//
