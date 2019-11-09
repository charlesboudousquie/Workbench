/*!***************************************************************************************
\file       Behavior.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node is the base class to represent decorative nodes with 1 child.
*****************************************************************************************/
#include "Behavior.hpp"
#include "../BehaviorSystem/BehaviorTree.hpp"
#include "BehaviorSystem/BehaviorTreeManager.hpp"
#include "Agent.hpp"

#include "../BehaviorSystem/BehaviorTreeDataBase.hpp"

void Behavior::printDebug(DEBUG_MESSAGE_TYPE type)
{
    std::string message("[DEBUG] ");

    switch (type)
    {
    case DEBUG_MESSAGE_TYPE::INIT:
        message += "Initing ";
        break;
    case DEBUG_MESSAGE_TYPE::UPDATE:
        message += "Updating ";
        break;
    case DEBUG_MESSAGE_TYPE::EXIT:
        message += "Exiting ";
        break;
    default:
        throw std::runtime_error("Debugging unknown behavior");
    }

    message += "Behavior " + std::to_string(this->getId()) + ": " + this->getName();

    BehaviorLogger::GetInstance().addMessage(message, this->getActor()->getName());
}

void Behavior::Exit()
{
    // when exiting a node, the state related to said node is removed, (except the result of that behavior)
    Behavior::GiveToParent(GetTask());

#ifdef DEBUGGING_NODES
    //"[ERROR] Unable to find object with given id: %lu \n"
    printDebug(DEBUG_MESSAGE_TYPE::EXIT);
#endif

}

BehaviorTaskPtr Behavior::GetTask()
{
    // actual task is stored in tree so we get it from there
    return this->getActor()->getComponent<Agent>()->GetTask();
}

gameObjectGatherer * Behavior::getObjectGatherer()
{
    return this->behaviorTreeOwner->GetManager()->getGO_Gatherer();
}

space* Behavior::getCurrentSpace()
{
    return this->getActor()->getSpace();
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

}

void Behavior::updateFromTypeRT(typeRT & p_data)
{
    throw std::runtime_error("Do not use update from type rt");
}

void Behavior::updateFromFile(const rapidjson::Value &)
{
}

void Behavior::setParent(BehaviorPtr parent_)
{
    this->parent = parent_;
}

void Behavior::setParentTree(BehaviorTree* pt)
{
    this->behaviorTreeOwner = pt;
}

BehaviorPtr Behavior::getParent()
{
    return parent;
}

std::shared_ptr<gameObject> Behavior::getActor()
{
    return ActorDatabase::GetInstance().GetCurrentActor();
}

Blackboard& Behavior::GetBlackboard()
{
    return getActor()->getComponent<Agent>()->GetBlackboard();
}

void Behavior::TakeTask(BehaviorTaskPtr t)
{
  t->SetCurrentBehavior(this);
  t->SetParentBehavior(this->parent.get());
  t->SetChildBehavior(this->getCurrentChild().get());
}

void Behavior::GiveToChild(BehaviorTaskPtr t)
{
    auto child = getCurrentChild();
    // add new state for the child to mess with
    t->RegisterNewNode(child->getId());
    // give task pointer to child
    child->TakeTask(t);
}

void Behavior::GiveToParent(BehaviorTaskPtr t)
{
    // remove state related to this behavior
    t->Pop_State();
    
    // if parent exists then give task to them
    if (parent)
    {
        parent->TakeTask(t);
    }
}

typeRT Behavior::toTypeRT() const
{
    return typeRT();
}
//
//void SetSpecialRenderData(const rapidjson::Value & JSON, typeRT & data, const char* member_name)
//{
//    // if json does not have attack damage variable then we are screwed
//    if (JSON.HasMember(member_name))
//    {
//        // set typert to have max time value
//        data.member(member_name).setFloat(JSON[member_name].GetFloat());
//        data.member("").set
//    }
//    else
//    {
//        throw std::runtime_error("Missing " + std::string(member_name));
//    }
//}
