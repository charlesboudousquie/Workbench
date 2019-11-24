/*!***************************************************************************************
\file       FindPathToPlayer.cpp
\author     Charles Boudousquie
\date       11/22/2019
\copyright  All content  2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Finds path to player from actor's current position.
*****************************************************************************************/
#include "FindPathToPlayer.hpp"
#include "DynamicWaypoints/WaypointGraphPathfinder.hpp"

void FindPathToPlayer::Init()
{
    // get waypointgraphpathfinder
    auto pathFinder = getpathfinder;
    
    // get npc/actor
    auto actor = getActor();
    
    // get our position
    auto ourPos = GetPosition(actor);
    
    // find player object
    auto playerObj = GetPlayer(this->getGameObjectGatherer());
    auto playerPos = GetPosition(playerObj);
    
    // get path
    std::shared_ptr<graphPath> pathToPlayer = this->GetPathFinder()->getPath(ourPos, playerPos);
    
    // get squad commander
    auto squad = this->GetFormationsManager()->GetSquad(actor->getID());
    
    // tell squad about the new path to follow
    squad->SetPath(pathToPlayer);
    
    GetTask()->SetPhase(BehaviorPhase::DONE);
    GetTask()->SetResult(BehaviorResult::SUCCESS);
    
}

