/*!***************************************************************************************
\file       Sequencer.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node returns true if all children succeed. Failure if one fails
*****************************************************************************************/
#include "Sequencer.hpp"

typeRT Sequencer::onRender()
{
    return Composite::compositeOnRender();
}

void Sequencer::Update(float dt)
{
    auto task = GetTask();
    auto result = task->GetResult();
    
    // if our child succeeded then move onto next one
    if (result == BehaviorResult::SUCCESS)
    {
        // in each node we keep track of which child index we are using
        task->IncrementChildIndex();// childIndices.top()++;

        // if done with all child nodes then we succeeded
        if (task->GetChildIndex() == this->childNodes.size())
        {
            task->SetResult(BehaviorResult::SUCCESS);
            GiveToParent(task);
        }
        // if we are not done then we send it back down to the next child
        else
        {
            GiveToChild(task);
        }
    }
    else if (result == BehaviorResult::FAILURE)
    {
        task->SetResult(BehaviorResult::FAILURE);
        GiveToParent(task);
    }
}










