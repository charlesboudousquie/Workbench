/*!***************************************************************************************
\file       BehaviorTree.hpp
\author     Charles Boudousquie
\date       9/24/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the object contains Behavior Nodes.
*****************************************************************************************/
#pragma once
#include <memory>

#include "AgentEncapsulator.hpp"

//class BehaviorTaskProxy;
class BehaviorTask;
class Behavior;
class Agent;

typedef std::shared_ptr<BehaviorTask> BehaviorTaskPtr;
//typedef std::shared_ptr<BehaviorTaskProxy> BehaviorTaskProxyPtr;
typedef std::shared_ptr<Behavior> BehaviorPtr;
typedef std::shared_ptr<Agent> AgentPtr;

class BehaviorTree
{
    // map of behavior pointer to id
    std::map<BehaviorPtr, int> nodeIDs;

    // main node of the tree
    BehaviorPtr root;

    BehaviorTaskPtr task;
    //BehaviorTaskProxyPtr taskProxy;

    // name of tree
    std::string name;

    //void RecursivelySetProxy(BehaviorTaskProxyPtr proxy, BehaviorPtr node);

    //void ResetTree();
    void SetRoot(BehaviorPtr);

public:

    // get task proxy
    //BehaviorTaskProxyPtr GetProxyTask();

    BehaviorTaskPtr GetTask();
    void SetTask(BehaviorTaskPtr);

    BehaviorPtr GetRoot();

    BehaviorTree();

    void SetName(const std::string& newName);
    std::string GetName();
    
    // give the tree its root node
    void SetUpTree(BehaviorPtr, std::map<int, BehaviorPtr>);

};

