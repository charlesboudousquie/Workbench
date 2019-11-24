/*!***************************************************************************************
\file       ResumePathing.cpp
\author     Charles Boudousquie
\date       11/22/2019
\copyright  All content  2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Tells squad that the npc is ready to go back to formation.
*****************************************************************************************/
#include "ResumePathing.hpp"
#include "PathFollowScript.hpp"

void ResumePathing::Init()
{
    // get script
    auto pathingScript = getscript;
    
    // tell script to resume
    pathingScript->Resume();
    
    GetTask()->SetPhase(BehaviorPhase::DONE);
    GetTask()->SetResult(BehaviorResult::SUCCESS);
}
