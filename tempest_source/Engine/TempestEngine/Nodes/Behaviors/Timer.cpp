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
#ifndef TESTING_NODES

typeRT Timer::onRender()
{
    return Decorator::decoratorOnRender(); // change this
}

void Timer::Init()
{
    // do not call decorator's init, since we do not give task to child immediately
    
    // reset timer
    timer = 2.0f;

    // call update next frame
    GetTask()->SetPhase(BehaviorPhase::PROGRESSING);
}

void Timer::Update(float dt)
{
    // update timer
    timer -= dt;

    // if timer finished then activate child node
    if (timer <= 0)
    {
        // we will be done when the child hands back the task to us
        GetTask()->SetPhase(BehaviorPhase::DONE);

        // send task to child so child can take care of it
        GiveToChild(GetTask());
    }
}

void Timer::Exit()
{
    Decorator::Exit();
}

#endif