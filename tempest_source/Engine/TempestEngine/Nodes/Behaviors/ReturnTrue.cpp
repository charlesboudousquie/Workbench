/*!***************************************************************************************
\file       ReturnTrue.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node returns true regardless of result of child.
*****************************************************************************************/
#include "ReturnTrue.hpp"

void ReturnTrue::handleResult(BehaviorResult)
{
    // get child phase
    BehaviorPhase childPhase = child->GetPhase();

    // dont care if they succeeded as long as they are done
    if (childPhase == BehaviorPhase::DONE)
    {
        // dont care if child finished or not
        this->phase = BehaviorPhase::DONE;
        this->result = BehaviorResult::SUCCESS;
    }
}

typeRT ReturnTrue::onRender()
{
    return Decorator::decoratorOnRender();
}

void ReturnTrue::Update(float dt)
{
  // activate child
  child->tick(dt);
}

