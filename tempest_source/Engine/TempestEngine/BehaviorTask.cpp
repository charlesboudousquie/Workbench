/*!***************************************************************************************
\file       BehaviorTask.cpp
\author     Charles Boudousquie
\date       10/17/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This is a data structure that keeps track of an agent's state within
            an instance of a BehaviorTree.
*****************************************************************************************/
#include "BehaviorTask.hpp"
#include "BehaviorTree.hpp"

#include "Agent.hpp"
#include "../Nodes/Behaviors/Behavior.hpp"

//void BehaviorTask::PushChildIndex(int newIndex)
//{
//
//    currentChildStack.push(newIndex);
//}

void BehaviorTask::Push_State(BehaviorState state)
{
    history.push(state);
}

void BehaviorTask::Pop_State()
{
    history.pop();
}

void BehaviorTask::IncrementChildIndex()
{
    history.top().childIndex++;
    //currentChildStack.top().++;
}

int BehaviorTask::GetChildIndex()
{
    return history.top().childIndex;
}

//void BehaviorTask::PopChildIndex()
//{
//    currentChildStack.pop();
//}

void BehaviorTask::SetResult(BehaviorResult r)
{
    result = r;
}

BehaviorResult BehaviorTask::GetResult()
{
    return result;
}

// usually want to set phase during update() or exit()
void BehaviorTask::SetPhase(BehaviorPhase p)
{
    history.top().phase = p;
    //assert(phases.top() == p);// this better not be modifying a temporary
}

//// usually want to push a new phase during init()
//void BehaviorTask::Push_Phase(BehaviorPhase p)
//{
//    phases.push(p);
//}

BehaviorPhase BehaviorTask::GetPhase()
{
    return history.top().phase;
}
//
//// only called when going back up to parent node
//void BehaviorTask::Pop_Phase()
//{
//    phases.pop();
//}

BehaviorTask::AgentPtr BehaviorTask::GetAgent()
{
    return agent;
}

void BehaviorTask::SetAgent(AgentPtr agent_)
{
    agent = agent_;
}


void BehaviorTask::SetChildBehavior(Behavior* b)
{
    childBehavior = b;
}

Behavior* BehaviorTask::GetChildBehavior()
{
    return childBehavior;
}

void BehaviorTask::SetParentBehavior(Behavior* b)
{
    parentBehavior = b;
}

Behavior* BehaviorTask::GetParentBehavior()
{
    return parentBehavior;
}


void BehaviorTask::SetCurrentBehavior(Behavior* b)
{
    currentBehavior = b;
}

Behavior* BehaviorTask::GetCurrentBehavior()
{
    return currentBehavior;
}

void BehaviorTask::SetTreePtr(BehaviorTreePtr t)
{
    this->tree = t;
}

BehaviorTreePtr BehaviorTask::GetTree()
{
    return tree;
}
