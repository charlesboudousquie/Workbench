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

#include "Nodes/TemplateNode.hpp"

class Behavior;
class AgentEncapsulator;
class BehaviorTree;

typedef std::shared_ptr<Behavior> BehaviorPtr;
typedef std::shared_ptr<BehaviorTree> BehaviorTreePtr;

enum class BehaviorResult { SUCCESS = 0, RUNNING, FAILURE };

enum class BehaviorPhase
{
    STARTING, // we can indeed enter the node
    PROGRESSING, // node is in the middle of execution
    WAITING, // node is inactive
    DONE // node either succeeded or failed
};

class Behavior : public Node
{
protected:

    // what node owns us
    BehaviorPtr parent;

    // result from the tick operation
    BehaviorResult result;

    // phase we are currently in
    BehaviorPhase phase;

    // by default it sets Phase to Progressing and Result to Running
    virtual void Init();
    // does nothing by default
    virtual void Update(float dt) = 0;
    // does nothing by default
    virtual void Exit();

    AgentEncapsulator* proxy;

    BehaviorTreePtr parentTree;
public:

    // set result status in typert
    void SetResultTypeRT(typeRT & p_data);

    // internal data of node
    typeRT data;

public:

    Behavior();

    void setAgentProxy(AgentEncapsulator*);

    // set phase of yourself, if you have child nodes
    // then you probably want to override this
    virtual void SetPhase(BehaviorPhase);
    virtual void SetResult(BehaviorResult);
    
    // get current result of node
    BehaviorResult getResult();

    BehaviorPhase GetPhase();

    // get currently active child of said node
    virtual BehaviorPtr getCurrentChild() = 0;

    // get all child nodes
    virtual std::vector<BehaviorPtr> GetChildren() = 0;

    // sets parent of behavior
    virtual void setParent(BehaviorPtr);

    // retrieve parent node, there should only be 1 parent
    BehaviorPtr getParent();

    // allows node to talk to its parent tree
    void setParentTree(BehaviorTreePtr);


    // based on what phase we are in, this will call Init, Update, or Exit.
    // Result of those functions is returned.
    void tick(float dt);

    // not sure what to do with this one
    // ask ryan
    virtual std::pair<bool, std::string> validate() { return {}; }

    // Update node's data based on info given, by default does nothing
    virtual void updateFromTypeRT(typeRT & p_data);

    // decorator has 1 chid, composite have multiple, leaves have none
    virtual void addChild(BehaviorPtr) = 0;

    // handle result of child
    virtual void handleResult(BehaviorResult) = 0;

    // tells whether or not the node is in the middle of an operation
    bool progressing();
};
