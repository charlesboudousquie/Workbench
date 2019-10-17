/*!***************************************************************************************
\file       RepeatUntilSuccess.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This class represents a node that continually activates its child until
            it succeeds.
*****************************************************************************************/
#include "RepeatUntilSuccess.hpp"

void RepeatUntilSuccess::handleResult(BehaviorResult)
{
    auto childResult = child->getResult();

    // if child succeeeded then we are done
    if (childResult == BehaviorResult::SUCCESS)
    {
        this->phase = BehaviorPhase::DONE;
        this->result = BehaviorResult::SUCCESS;
    }
    else if (childResult == BehaviorResult::FAILURE)
    {
        // restart child
        child->SetPhase(BehaviorPhase::STARTING);
    }
}

typeRT RepeatUntilSuccess::onRender()
{
    return Decorator::decoratorOnRender();
}

void RepeatUntilSuccess::Update(float dt)
{
    child->tick(dt);
}
