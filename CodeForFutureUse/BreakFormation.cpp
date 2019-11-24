/*!***************************************************************************************
\file       BreakFormation.cpp
\author     Charles Boudousquie
\date       11/22/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This behavior tells npc to break away from his squad.
*****************************************************************************************/
#include "BreakFormation.hpp"
#include "Formations/FormationsManager.hpp"

void BreakFormation::Update(float)
{
    auto formManager = GetManager();
    
    auto actor = getActor();
    
    // SEE if we can join a squad or make a new one
    formManager->LeaveSquad(actor->getID());
    
    GetTask()->SetPhase(BehaviorPhase::DONE);
    GetTask()->SetResult(BehaviorResult::SUCCESS);
}
