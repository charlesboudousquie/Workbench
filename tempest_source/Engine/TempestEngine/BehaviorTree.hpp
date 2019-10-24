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

class BehaviorTask;
class Behavior;

typedef std::shared_ptr<BehaviorTask> BehaviorTaskPtr;
typedef std::shared_ptr<Behavior> BehaviorPtr;

class BehaviorTree
{
    // map of behavior pointer to id
    std::map<BehaviorPtr, int> nodeIDs;

    // main node of the tree
    BehaviorPtr root;

    BehaviorTaskPtr task;

    // name of tree
    std::string name;

    //void SetRoot(BehaviorPtr);

    // for each behavior set its parent tree to this
    void RecursiveSetParentTree(Behavior*);

public:

    BehaviorTaskPtr GetTask();
    void SetTask(BehaviorTaskPtr);

    BehaviorPtr GetRoot();

    BehaviorTree();

    void SetName(const std::string& newName);
    std::string GetName();
    
    // give the tree its root node
    void SetUpTree(BehaviorPtr, std::map<int, BehaviorPtr>);

};

