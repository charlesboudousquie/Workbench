/*!***************************************************************************************
\file       RepeatUntilN.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node fully processes its child exactly N times aka N SUCCESSes.
*****************************************************************************************/
#include "RepeatUntilN.hpp"

//void RepeatUntilN::updateFromTypeRT(typeRT & data)
//{
//    this->counter = data.member("counter").getInt();
//    this->maximumSuccesses = data.member("maximumSuccesses").getInt();
//}

//typeRT RepeatUntilN::onRender() 
//{
//    //typeRT data;
//    //data.setTypeName("RepeatUntilN");
//    //data.insertMember(typeRT("counter", counter));
//    //data.insertMember(typeRT("maximumSuccesses", maximumSuccesses));
//
//   /*     result.setTypeName("animation");
//    result.setVariableName(m_tag);
//    result.insertMember(typeRT("tag", m_tag));
//    result.insertMember(typeRT("frameBegin", unsigned(m_frameBegin)));
//    result.insertMember(typeRT("frameEnd", unsigned(m_frameEnd)));
//    result.insertMember(typeRT("fps", m_fps));
//    result.insertMember(typeRT("loop", m_loop));*/
//
//    return data;
//}

void RepeatUntilN::handleResult(BehaviorResult)
{
    // if done then exit node
    if (counter == this->maximumSuccesses)
    {
        this->result = BehaviorResult::SUCCESS;
        this->phase = BehaviorPhase::DONE;
    }
    else
    {
        // activate child behavior
        BehaviorResult childResult = child->getResult();

        // if child succeeded then we are closer to success
        if (childResult == BehaviorResult::SUCCESS)
        {
            counter++;

            // make sure child does behavior again
            child->SetPhase(BehaviorPhase::STARTING);
        }
        // if child failed then it means we could not complete a behavior N times
        else if (childResult == BehaviorResult::FAILURE)
        {
            this->phase = BehaviorPhase::DONE;
            this->result = BehaviorResult::FAILURE;
        }
    }

}

typeRT RepeatUntilN::onRender()
{
    return Decorator::decoratorOnRender();
}

void RepeatUntilN::Init()
{
    // progressing and running, child is ready
    Decorator::Init();
    counter = 0;
}

void RepeatUntilN::Update(float dt)
{
    child->tick(dt);
    
}
