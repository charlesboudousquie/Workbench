/*!***************************************************************************************
\file       DefaultLeaf.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This class represents a default leaf node for testing purposes. It always succeeds.
*****************************************************************************************/
#include "DefaultLeaf.hpp"

typeRT DefaultLeaf::onRender()
{
    return Leaf::leafOnRender();
}

void DefaultLeaf::Update(float dt)
{
    this->GetTask()->SetPhase(BehaviorPhase::DONE);
    this->GetTask()->SetResult(BehaviorResult::SUCCESS);
    /*this->result = BehaviorResult::SUCCESS;
    this->phase = BehaviorPhase::DONE;*/
}
