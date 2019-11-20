/*!***************************************************************************************
\file       Regroup.cpp
\author     Charles Boudousquie
\date       11/19/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This behavior tells npc to regroup with some squad.
*****************************************************************************************/
#include "Regroup.hpp"

void Regroup::Update(float)
{
    auto formManager = GetManager();
    
    auto actor = getActor();
    
    // SEE if we can join a squad or make a new one
    if(formManager->JoinSquad(actor->getID()))
    {
        GetTask()->SetPhase(BehaviorPhase::DONE);
        GetTask()->SetResult(BehaviorResult::SUCCESS);
    }
    else
    {
        // we should not get here
        GetTask()->SetPhase(BehaviorPhase::DONE);
        GetTask()->SetResult(BehaviorResult::FAILURE);
    }
    
}