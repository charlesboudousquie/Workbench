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

void CreateSquad(objID unit)
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
        if(faction == squad.GetFaction())
        {
            bool foundSquad = squad.JoinSquad(unit);
            if(foundSquad)
            {
                break;
            }
                
        }
    }
    
    // if squad was not found then we create our own
    if(!squadFound)
    {
        
    }
}


void LeaveSquad()
{
    
}