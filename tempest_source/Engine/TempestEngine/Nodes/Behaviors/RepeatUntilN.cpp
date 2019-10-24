/*!***************************************************************************************
\file       RepeatUntilN.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node fully processes its child exactly N times aka N SUCCESSes.
*****************************************************************************************/
#include "RepeatUntilN.hpp"
#ifndef TESTING_NODES

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
    auto task = GetTask();
    // if done then exit node
    if (counter == this->maximumSuccesses)
    {
        task->SetResult(BehaviorResult::SUCCESS);
        GiveToParent(task);
    }
    else
    {
        // activate child behavior
        BehaviorResult childResult = task->GetResult();

        // if child succeeded then we are closer to success
        if (childResult == BehaviorResult::SUCCESS)
        {
            counter++;
            GiveToChild(task); // give task back to child again
        }
        // if child failed then it means we could not complete a behavior N times
        else if (childResult == BehaviorResult::FAILURE)
        {
            task->SetResult(BehaviorResult::FAILURE);
            GiveToParent(task);
        }
    }
}
#endif