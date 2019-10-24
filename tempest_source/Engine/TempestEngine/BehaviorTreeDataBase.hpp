///*!***************************************************************************************
//\file       BehaviorTreeDatabase.hpp
//\author     Charles Boudousquie
//\date       10/17/2019
//\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
//\par        Project: Boomerang
//\brief      This is the interface for the Behavior Tree Database that keeps track of agents
//            and their corresponding trees. Also keeps track of tasks.
//*****************************************************************************************/
//#pragma once
//#include <vector>
//#include <set>
//#include <unordered_map>
//#include <string>
//#include <memory>
////#include "Agent.hpp"
////#include "IComponentRepository.hpp"
//
//// purpose of class is to have an object that keeps track of:
//// *which agent goes to what tree
//// *link agent to tree
//// *shift agent to new tree if necessary
//// *keep a list of tasks that need to be done
//// *link agent to behavior id of its coresponding tree
//
//
//// Created because:
//// multiple agents per tree
//// need to get tree from agent
//// need to set an agent to a tree
//// need some area to store all the actual trees
//// need a list of tasks to iterate over
//// need a list of agents
//// need to get list of Agents that use a specific tree
//
//class BehaviorTree;
//typedef std::shared_ptr<BehaviorTree> BehaviorTreePtr;
//
////class Agent;
////typedef std::shared_ptr<Agent> AgentPtr;
////typedef componentHandle<Agent> AgentPtr;
//
//class gameObject;
//typedef std::shared_ptr<gameObject> GameObjectPtr;
//
//class BehaviorTask;
//typedef std::shared_ptr<BehaviorTask> BehaviorTaskPtr;
//
//class Behavior;
//
//class BehaviorTreeDataBase
//{
//    typedef std::string TreeName;
//    typedef std::string AgentName;
//
//    // the list of trees by name
//    std::unordered_map<TreeName, BehaviorTreePtr> trees;
//
//    // list of tasks to iterate over
//    std::unordered_map<GameObjectPtr, BehaviorTaskPtr> tasks;
//
//    // create a task that links together an Agent and a Tree
//    BehaviorTaskPtr CreateTask(GameObjectPtr, BehaviorTreePtr);
//
//    // remove agent from specific tree
//    void UnassignActorFromTree(GameObjectPtr);
//
//public:
//    // NOTE: Agents should only be assigned to 1 tree at any moment in time.
//
//    // assign an agent to a tree, note that if agent is
//    // already assigned to a tree he will be removed
//    // from the old one and assigned to the new one
//    void AssignActorToTree(GameObjectPtr);
//
//    // add tree to database
//    void AddTree(BehaviorTreePtr, const std::string&);
//
//    // get corresponding tree from a particular agent
//    BehaviorTreePtr GetTreeFromActor(GameObjectPtr);
//
//    // get tree from name
//    BehaviorTreePtr GetTree(const std::string&);
//
//    // check if tree exists
//    bool HasTree(const std::string&);
//
//    // checks is actor exists in database
//    bool HasActor(GameObjectPtr);
//
//    // removes actor from database
//    void RemoveActor(GameObjectPtr);
//
//    // get all trees
//    std::unordered_map<TreeName, BehaviorTreePtr> GetAllTrees();
//
//    // get list of all tasks in database
//    std::set<BehaviorTaskPtr> GetTasks();
//
//    // get behavior node that a specific agent is working with
//    Behavior* ActorGetActiveBehavior(GameObjectPtr);
//
//};
//
//
