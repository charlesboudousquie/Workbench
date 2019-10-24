/*!***************************************************************************************
\file       ReturnTrue.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node returns true regardless of result of child.
*****************************************************************************************/
#include "ReturnTrue.hpp"
#ifndef TESTING_NODES

typeRT ReturnTrue::onRender()
{
    return Decorator::decoratorOnRender();
}

void ReturnTrue::Update(float dt)
{
    // child is done at this point
    GetTask()->SetResult(BehaviorResult::SUCCESS);
    GiveToParent(GetTask());
}

#endif