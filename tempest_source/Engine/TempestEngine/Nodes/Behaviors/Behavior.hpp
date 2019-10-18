/*!***************************************************************************************
\file       Behavior.hpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node is the abstract class from which all behaviors derive from.
*****************************************************************************************/
#pragma once

#include "Nodes/Node.hpp"
#include "BehaviorEnums.hpp"
#include "BehaviorTask.hpp"

#define TESTING_NODES // used when just testing small amount of nodes

class Behavior;
class AgentEncapsulator;
class BehaviorTree;

typedef std::shared_ptr<Behavior> BehaviorPtr;
typedef std::shared_ptr<BehaviorTree> BehaviorTreePtr;
typedef std::shared_ptr<BehaviorTask> BehaviorTaskPtr;

class Behavior : public Node
{
protected:

    // what node owns us
    BehaviorPtr parent;
    BehaviorTreePtr parentTree;

    // by default it sets Phase to Progressing and Result to Running
    virtual void Init() = 0;
    // does nothing by default
    virtual void Update(float dt) = 0;
    // does nothing by default
    virtual void Exit();

    // actual type of node
    virtual BehaviorType GetType() = 0;

    // shorthand to get task, it
    // is essentially a helper function to get 
    // task from parent tree
    BehaviorTaskPtr GetTask();

    // These 3 functions are meant to pass around a task within the 
    // tree. The behaviors themselves should call TakeTask, GiveToChild, and GiveToParent
    // As for the task proxy, it is assigned from outside and it is meant to
    // merely switch out the task that is being passed around.
    // So in short: the three functions below move a task through the tree,
    // while the task proxy dictates who that task is.
    void TakeTask(BehaviorTaskPtr);
    void GiveToChild(BehaviorTaskPtr);
    void GiveToParent(BehaviorTaskPtr);

public:
    Behavior();

    // set result status in typert
    void SetResultTypeRT(typeRT & p_data);

    // internal data of node
    typeRT data;

    // get currently active child of said node
    virtual BehaviorPtr getCurrentChild() = 0;

    // get all child nodes
    virtual std::vector<BehaviorPtr> GetChildren() = 0;

    // sets parent of behavior
    virtual void setParent(BehaviorPtr);

    // retrieve parent node, there should only be 1 parent
    BehaviorPtr getParent();

    // based on what phase we are in, this will call Init, Update, or Exit.
    void tick(float dt);

    // not sure what to do with this one ask ryan
    virtual std::pair<bool, std::string> validate() { return {}; }

    // Update node's data based on info given, by default does nothing
    virtual void updateFromTypeRT(typeRT & p_data);

    // decorator has 1 chid, composite have multiple, leaves have none
    virtual void addChild(BehaviorPtr) = 0;

};
