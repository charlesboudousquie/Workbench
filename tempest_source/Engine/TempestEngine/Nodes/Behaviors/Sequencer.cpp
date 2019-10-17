/*!***************************************************************************************
\file       Sequencer.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node returns true if all children succeed. Failure if one fails
*****************************************************************************************/
#include "Sequencer.hpp"

void Sequencer::handleResult(BehaviorResult childResult)
{
    // if child failed then we failed
    if (childResult == BehaviorResult::FAILURE)
    {
        this->phase = BehaviorPhase::DONE;
        this->result = BehaviorResult::FAILURE;
    }
    else if (childResult == BehaviorResult::SUCCESS)
    {
        // if child succeeded then move onto next one
        currentChild++;

        // if at end then we succeeded
        if (currentChild == childNodes.end())
        {
            this->phase = BehaviorPhase::DONE;
            this->result = BehaviorResult::SUCCESS;
        }
    }
}

typeRT Sequencer::onRender()
{
    return Composite::compositeOnRender();
}

void Sequencer::Update(float dt)
{
    (*currentChild)->tick(dt);
}
