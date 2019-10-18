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
//#include "BehaviorTask.hpp"

//
//void Behavior::Init()
//{
//    //// set Result to running
//    //result = BehaviorResult::RUNNING;
//    //// set Phase to Progressing
//    //phase = BehaviorPhase::PROGRESSING;
//}

void Behavior::Exit()
{
    // when exiting a node, the state related to said node is removed, (except the result of that behavior)
    GetTask()->Pop_State();
}

BehaviorTaskPtr Behavior::GetTask()
{
    // actual task is stored in tree so we get it from there
    return this->parentTree->GetTask();
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

void Behavior::tick(float dt)
{
    auto phase = GetTask()->GetPhase();
    // if just starting up then we need to initialize
    if (phase == BehaviorPhase::STARTING)
    {   
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

void Behavior::TakeTask(BehaviorTaskPtr t)
{
  t->SetCurrentBehavior(this);
  t->SetParentBehavior(this->parent.get());
  t->SetChildBehavior(this->getCurrentChild().get());
}

void Behavior::GiveToChild(BehaviorTaskPtr t)
{
    // give task pointer to child
    getCurrentChild()->TakeTask(t);
}

void Behavior::GiveToParent(BehaviorTaskPtr t)
{
    // if parent exists then give task to them
    if (parent)
    {
        parent->TakeTask(t);
    }
}

