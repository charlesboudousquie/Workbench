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
#ifndef TESTING_NODES

typeRT RepeatUntilSuccess::onRender()
{
    return Decorator::decoratorOnRender();
}

void RepeatUntilSuccess::Update(float dt)
{
    auto task = GetTask();
    auto childResult = task->GetResult();

    // if child succeeeded then we are done
    if (childResult == BehaviorResult::SUCCESS)
    {
        task->SetResult(BehaviorResult::SUCCESS);
        GiveToParent(task);
    }
    else if (childResult == BehaviorResult::FAILURE)
    {
        GiveToChild(task); // tell child to try again
    }
}
#endif TESTING_NODES

