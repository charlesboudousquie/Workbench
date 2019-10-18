/*!***************************************************************************************
\file       Sequencer.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node returns true if all children succeed. Failure if one fails
*****************************************************************************************/
#include "Sequencer.hpp"

//void Sequencer::handleResult(BehaviorResult childResult)
//{
//    auto task = GetTask();
//
//    // if child failed then we failed
//    if (childResult == BehaviorResult::FAILURE)
//    {
//        task->SetPhase(BehaviorPhase::DONE);
//        task->SetResult(BehaviorResult::FAILURE);
//        this->GiveToParent(task);
//        //this->phase = BehaviorPhase::DONE;
//        //this->result = BehaviorResult::FAILURE;
//    }
//    else if (childResult == BehaviorResult::SUCCESS)
//    {
//        // if child succeeded then move onto next one
//        currentChild++;
//
//        // if at end then we succeeded
//        if (currentChild == childNodes.end())
//        {
//            this->phase = BehaviorPhase::DONE;
//            this->result = BehaviorResult::SUCCESS;
//        }
//    }
//}

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
            // Setting phase doesnt matter since we are leaving
            // this node.
            GiveToParent(task);
        }
    }
    else if (result == BehaviorResult::FAILURE)
    {
        task->SetResult(BehaviorResult::FAILURE);
        GiveToParent(task);
    }
}










