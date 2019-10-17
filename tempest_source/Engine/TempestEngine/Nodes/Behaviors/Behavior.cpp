/*!***************************************************************************************
\file       Behavior.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node is the base class to represent decorative nodes with 1 child.
*****************************************************************************************/
#include "Behavior.hpp"
#include "BehaviorTree.hpp"
#include "AgentEncapsulator.hpp"

void Behavior::Init()
{
    // set Result to running
    result = BehaviorResult::RUNNING;
    // set Phase to Progressing
    phase = BehaviorPhase::PROGRESSING;
}

void Behavior::Exit()
{

}

void Behavior::SetResultTypeRT(typeRT & p_data)
{
    //if()
    // TODO
    //p_data.member("NodeResult").setInt((int)this->result);
    //p_data.member("nodeID").setInt(this->getId());
}

Behavior::Behavior()
{
    // Undero NO circumstances should a behavior node have more than 1 input
    setIntputs(1);
}

void Behavior::setAgentProxy(AgentEncapsulator* proxy_)
{
    proxy = proxy_;
}

void Behavior::SetPhase(BehaviorPhase phase_)
{
    phase = phase_;
}

void Behavior::SetResult(BehaviorResult result_)
{
    result = result_;
}

BehaviorResult Behavior::getResult()
{
    return result;
}

BehaviorPhase Behavior::GetPhase()
{
    return this->phase;
}

void Behavior::tick(float dt)
{
    // if just starting up then we need to initialize
    if (phase == BehaviorPhase::STARTING)
    {
        // make sure parent tree actually knows that this node is active
        parentTree->SetCurrentNode(this->getId());
        
        // this pointer should act such that if subclasses
        // override init function then that function will be
        // called instead
        this->Init();
    }
    if (phase == BehaviorPhase::PROGRESSING)
    {
        this->Update(dt);
    }
    if (phase == BehaviorPhase::DONE)
    {
        this->Exit();
    }

    // set result information in type rt for later use
    this->SetResultTypeRT(data);
}

void Behavior::updateFromTypeRT(typeRT & p_data)
{
}

void Behavior::setParent(BehaviorPtr parent_)
{
    this->parent = parent_;
}

BehaviorPtr Behavior::getParent()
{
    return parent;
}

void Behavior::setParentTree(BehaviorTreePtr parentTree_)
{
    parentTree = parentTree_;
}

bool Behavior::progressing()
{
    return this->phase == BehaviorPhase::PROGRESSING;
}
