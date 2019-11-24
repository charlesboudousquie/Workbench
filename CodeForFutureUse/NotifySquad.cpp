/*!***************************************************************************************
\file       PausePathing.cpp
\author     Charles Boudousquie
\date       11/22/2019
\copyright  All content  2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Tells squad that the npc wishes to break from formation.
*****************************************************************************************/
#include "PausePathing.hpp"
#include "PathFollowScript.hpp"

void PausePathing::Init()
{
    
    // notify squad that unit is engaging enemy
    auto squad = getSquad();
    
    squad->MadeContact
    
    GetTask()->SetPhase(BehaviorPhase::DONE);
    GetTask()->SetResult(BehaviorResult::SUCCESS);
    
}


