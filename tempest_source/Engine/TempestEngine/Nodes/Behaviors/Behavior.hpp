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
#include "GameObject.hpp"

//#define TESTING_NODES // used when just testing small amount of nodes

class Behavior;
class BehaviorTree;


typedef std::shared_ptr<Behavior> BehaviorPtr;
typedef std::shared_ptr<BehaviorTree> BehaviorTreePtr;
typedef std::shared_ptr<BehaviorTask> BehaviorTaskPtr;

class Behavior : public Node
{
protected:

    // what node owns us
    BehaviorPtr parent;
    BehaviorTree* parentTree;

    // can be overriden
    virtual void Init() = 0;
    // MUST be overriden
    virtual void Update(float dt) = 0;
    // by default is just hands task to parent (it calss GiveToParent)
    virtual void Exit();

    // actual type of node
    virtual BehaviorType GetType() = 0;

    // shorthand to get task, it
    // is essentially a helper function to get 
    // task from parent tree
    BehaviorTaskPtr GetTask();

    // this function gets the actor/npc that is
    // currently working with this node
    std::shared_ptr<gameObject> getActor();

public:
    // These 3 functions are meant to pass around a task within the 
    // tree. The behaviors or the manager should call TakeTask, GiveToChild, and GiveToParent
    void TakeTask(BehaviorTaskPtr);
    void GiveToChild(BehaviorTaskPtr);
    void GiveToParent(BehaviorTaskPtr);

    Behavior();

    // set result status in typert
    void SetResultTypeRT(typeRT & p_data);

    // internal data of node
    typeRT data;

    // get currently active child of said node
    virtual BehaviorPtr getCurrentChild() = 0;

    // get all child nodes
    virtual std::vector<BehaviorPtr> GetChildren() = 0;

    void setParentTree(BehaviorTree*);

    // sets parent of behavior
    void setParent(BehaviorPtr);

    // retrieve parent node, there should only be 1 parent
    BehaviorPtr getParent();

    // get tree that behavior is a part of
    //BehaviorTreePtr getParentTree();

    // based on what phase we are in, this will call Init, Update, or Exit.
    // A behavior should never call tick, only the agent does
    void tick(float dt);

    // not sure what to do with this one ask ryan
    virtual std::pair<bool, std::string> validate() { return {}; }

    // Update node's data based on info given, by default does nothing
    virtual void updateFromTypeRT(typeRT & p_data);

    // decorator has 1 chid, composite have multiple, leaves have none
    virtual void addChild(BehaviorPtr) = 0;

};
