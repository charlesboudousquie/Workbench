/*!***************************************************************************************
\file       Timer.hpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node is a simple Decorator timer that activates its child node when
            timer hits 0.
*****************************************************************************************/
#include "Timer.hpp"

typeRT Timer::onRender()
{
    return Decorator::decoratorOnRender();
}

void Timer::handleResult(BehaviorResult)
{
    // if child is done then we are done and we set our result to be that of the child's
    if (child->GetPhase() == BehaviorPhase::DONE)
    {
        this->phase = BehaviorPhase::DONE;
        this->result = child->getResult();
        timer = 2.0f; // reset time
    }
}

void Timer::Update(float dt)
{
    timer -= dt;

    // if timer finished then activate child node
    if (timer <= 0)
    {
        child->tick(dt);
    }
}
