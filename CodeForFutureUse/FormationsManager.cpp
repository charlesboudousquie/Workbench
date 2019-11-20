/*!***************************************************************************************
\file       FormationsManager.cpp
\author     Charles Boudousquie
\date       11/19/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      Contains all squads within the game.
*****************************************************************************************/
#include "FormationsManager.hpp"
#include "GameObject.hpp"
#include "Allegiance.hpp"
#include "Factions.hpp"

void FormationsManager::CreateSquad(objID unit)
{
    
    
    // get faction of unit
    auto actor = getSystemManager()->....getGameObjectByID(unit);
    
    // get faction
    auto faction = actor->getComponent<Allegiance>()->getFaction();
    
    // get space
    auto m_space = getCurrentSpace();
    
    assert(faction != Faction::NONE);
    
    if(faction == Faction::PLAYER_MINION || faction == Faction::PLAYER)
    {
        // currentspace, maximum rings, faction
        squads.push_back(m_space, 100000, faction);
    }
    else
    {
        squads.push_back(m_space, 1, faction);
    }
    
}

void FormationsManager::JoinSquad(objID unit)
{
    
    // get faction of unit
    auto actor = getSystemManager()->....getGameObjectByID(unit);
    
    // get faction
    auto faction = actor->getComponent<Allegiance>()->getFaction();
    
    bool squadFound = false;
    
    // find an allied squad
    for(auto & squad : squads)
    {
        // if allied with squad
        if(faction == squad.GetFaction())
        {
            // if squad has room
            if(!squad.Full())
            {
                squad.JoinSquad(unit);
                break;
            }
                
        }
    }
    
    // if squad was not found then we create our own
    if(!squadFound)
    {
        // create and append squad to back
        CreateSquad(unit);
    }
}


void FormationsManager::LeaveSquad(objID unit)
{
    // get faction of unit
    auto actor = getSystemManager()->....getGameObjectByID(unit);
    
    // get faction
    auto faction = actor->getComponent<Allegiance>()->getFaction();
    
    bool squadFound = false;
    
    for(auto & squad : squads)
    {
        if(faction == squad.GetFaction())
        {
            
            if(squad.HasUnit(unit))
            {
                squadFound = true;
                squad.LeaveSquad(unit);
                break;
            }
                
        }
    }
    
    // if we tried to leave a squad that we were not a part of then report this error
    if(!squadFound)
    {
        logger log("Formations Manager");
        
        auto log_error = log.error();
        log_error << " Actor: " << actor->getName() << ", With faction: " FactionToName(faction) << std::endl;
        log_error << " tried to leave his squad, but there was no squad containing him." << std::endl;
    }
}

















