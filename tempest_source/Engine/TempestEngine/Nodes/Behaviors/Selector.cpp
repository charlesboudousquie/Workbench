/*!***************************************************************************************
\file       ReturnTrue.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node returns true if one child succeeds, False if none succeed.
*****************************************************************************************/
#include "Selector.hpp"
#ifndef TESTING_NODES

void Selector::Update(float dt)
{
    // run current child
    (*currentChild)->tick(dt);
}

void Selector::handleResult(BehaviorResult childResult)
{
    // if child succeeded then we succeeded
    if (childResult == BehaviorResult::SUCCESS)
    {
        this->result = childResult;
        this->phase = BehaviorPhase::DONE;
    }
    if (childResult == BehaviorResult::FAILURE)
    {
        // if failed then go to next child
        currentChild++;

        // if no more child nodes left to try
        if (currentChild == childNodes.end())
        {
            this->phase = BehaviorPhase::DONE;
            this->result = BehaviorResult::FAILURE;
        }
    }
}

typeRT Selector::onRender()
{
    return Composite::compositeOnRender();
}

void Selector::Init()
{
    // set ourselves to being ready and progressing, child nodes
    // are also set as ready
    Composite::Init();
}

#endif