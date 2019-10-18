/*!***************************************************************************************
\file       BehaviorTask.hpp
\author     Charles Boudousquie
\date       10/17/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This is a data structure that keeps track of an agent's state within
            an instance of a BehaviorTree.
*****************************************************************************************/
#pragma once
#include <stack>
#include "BehaviorEnums.hpp"

class Agent;
class Behavior;
class BehaviorTree;

typedef int ChildIndex;
typedef int BehaviorID;

class BehaviorState
{
public:
    BehaviorState(BehaviorID id_, ChildIndex childIndex_,
        BehaviorPhase phase_/*, BehaviorResult result_*/)
        : id(id_), childIndex(childIndex_), phase(phase_)//, result(result_)
    {}

    BehaviorID id;
    ChildIndex childIndex;
    BehaviorPhase phase;
};

class BehaviorTask
{
public:

    typedef std::shared_ptr<Agent> AgentPtr;
    typedef std::shared_ptr<BehaviorTree> BehaviorTreePtr;

    // NOTE:
    // We want multiple agents to use the exact same tree. 
    // What I mean by that is that if multiple agents (like a "Guard" or 
    // "Soldier") use the exact same tree, then ideally only 1 instanciation of 
    // that tree would be in memory.
    // But at the same time the agent need to remember what they are
    // doing aka their state. 
    // So if multiple agents use the exact same tree then they CANNOT store 
    // their state in the tree.
    // Thus they should store their state inside their own personal blackboard.
    // So then one may ask "How do we keep track of where exactly this
    // agent is inside the tree?".
    // This is where the "Task" comes in. 
    // It keeps track of what agent it corresponds to, AND keeps track of where 
    // in the tree the agent last left off.

    // If you are wondering why the childIndices and phases are stacks,
    // it is due to the fact that an Agent MUST remember its previous
    // states higher up in the tree. Especially since the nodes in the
    // tree are NOT allowed to store state. The nodes in the Behavior Tree
    // are essentially glorified instructions through which an agent acts upon.

    // pushes a new state onto the history stack
    void Push_State(BehaviorState);
    void Pop_State();

    // add child index to keep track of
    //void PushChildIndex(int newIndex);
    // increment child index by 1
    void IncrementChildIndex();
    // Of the node we are currently in (like a sequncer),
    // this will tell us the index of the child node we 
    // are operating on.
    int GetChildIndex();
    //void PopChildIndex();

    void SetResult(BehaviorResult);
    BehaviorResult GetResult();

    // usually want to set phase during Behavior update() or Behavior exit()
    void SetPhase(BehaviorPhase);
    // usually want to push a new phase during Behavior init()
    //void Push_Phase(BehaviorPhase);
    BehaviorPhase GetPhase();
    // only called when going back up to parent node
    //void Pop_Phase();

    AgentPtr GetAgent();
    void SetAgent(AgentPtr);

    void SetChildBehavior(Behavior*);
    Behavior* GetChildBehavior();

    void SetParentBehavior(Behavior*);
    Behavior* GetParentBehavior();

    void SetCurrentBehavior(Behavior*);
    Behavior* GetCurrentBehavior();

    void SetTreePtr(BehaviorTreePtr);
    BehaviorTreePtr GetTree();

private:
    // parent node that we go to when 
    // done (potentially null if we are at the root).
    Behavior* parentBehavior;

    // Current Behavior Node we are operating on
    Behavior* currentBehavior;

    // The child of the current Node we are working
    // with (potentially null if we are a leaf node).
    Behavior* childBehavior;

    // The agent we will be working with
    AgentPtr agent;

    // the tree this task is working in
    BehaviorTreePtr tree;

    // NOTE: 
    // Since traversing a Behavior Tree is similar to depth
    // first search, we will be using stacks to remember
    // our state in higher nodes. This is helpful when an agent
    // finishes a particular behavior and needs to remember
    // what its last state was in the parent node.

    // last known result of operation
    BehaviorResult result;

    // history of states throughout while traversing the tree
    std::stack<BehaviorState> history;
};
