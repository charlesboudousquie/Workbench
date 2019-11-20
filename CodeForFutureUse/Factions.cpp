/*!***************************************************************************************
\file       Factions.cpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      Here is the enum class to represent what faction an npc is a part of.
*****************************************************************************************/
#include "Factions.hpp"
#include <map>
#include <unordered_map>
#include <set>


//                PLAYER, PLAYER_MINION, ENEMY, NONE
// PLAYER
// PLAYER_MINION
// ENEMY
// NONE



static std::unordered_map<Allegiance::Faction, std::set<Allegiance::Faction>> allies
{
    {Faction::PLAYER, {Faction::PLAYER, Faction::PLAYER_MINION}},
    {Faction::PLAYER_MINION, {Faction::PLAYER, Faction::PLAYER_MINION}}
};

bool FactionExists(const Faction& f)
{
    return allies.find(f) != allies.end();
}

std::map<Faction, const char*> factionNames
{
    {"PLAYER", Faction::PLAYER},
    {"PLAYER_MINION", Faction::PLAYER_MINION},
    {"ENEMY", Faction::ENEMY}
};

const char* FactionToName(const Faction& faction)
{
    if(factionNames.find(faction) != factionNames.end())
    {
        return factionNames[faction];
    }
    
    throw std::runtime_error("Tried to acces name of faction that did not exist");
}


bool IsAlly(Faction factionA, Faction factionB)
{
    assert(FactionExists(factionA) && FactionExists(factionB));
    
    const auto& allyList = allies[factionA];
    
    return allyList.find(factionB) != allyList.end();
    
}









