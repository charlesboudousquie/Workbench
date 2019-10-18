/*!***************************************************************************************
\file       RepeatUntilFailure.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This class represents a node that continually activates its child until 
            it fails.
*****************************************************************************************/
#include "RepeatUntilFailure.hpp"
#ifndef TESTING_NODES

void RepeatUntilFailure::handleResult(BehaviorResult)
{
    BehaviorResult childResult = child->getResult();

    // if child fails then mission accomplished
  if (childResult == BehaviorResult::FAILURE)
  {
      this->result = BehaviorResult::SUCCESS;
      this->phase = BehaviorPhase::DONE;
  }
  else if (childResult == BehaviorResult::SUCCESS)
  {
      // set our phase to restarting
      this->phase = BehaviorPhase::STARTING;

      // child succeeded meaning we need to restart
      child->SetPhase(BehaviorPhase::STARTING);
  }
}

typeRT RepeatUntilFailure::onRender()
{
    return Decorator::decoratorOnRender();
}

void RepeatUntilFailure::Update(float dt)
{
  // run child behavior
  child->tick(dt);
}

#endif