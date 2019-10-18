
/*!***************************************************************************************
\file       Inverter.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node inverts the result of its child node.
*****************************************************************************************/
#include "Inverter.hpp"
#ifndef TESTING_NODES

void Inverter::handleResult(BehaviorResult)
{
    // get result of child
    BehaviorResult childResult = child->getResult();

    // if child succeeds then we have failed
    if (childResult == BehaviorResult::SUCCESS)
    {
        this->result = BehaviorResult::FAILURE;
        this->phase = BehaviorPhase::DONE;
    }
    else if(childResult == BehaviorResult::FAILURE)
    {
        // child failed so return success
        this->result = BehaviorResult::SUCCESS;
        this->phase = BehaviorPhase::DONE;
    }
}

typeRT Inverter::onRender()
{
    return Decorator::decoratorOnRender();
}

void Inverter::Init()
{
    // need default initialization and need to start up child
    Decorator::Init();

    // we are ready to go
    this->phase = BehaviorPhase::PROGRESSING;
}

void Inverter::Update(float dt)
{
    child->tick(dt);
}

#endif