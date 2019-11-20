/*!***************************************************************************************
\file       Factions.hpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      Here is the enum class to represent what faction an npc is a part of.
*****************************************************************************************/
#pragma once

enum class Faction { PLAYER = 0, PLAYER_MINION, ENEMY, NONE, COUNT };

const char* FactionToName();

bool IsAlly(Faction factionA, Faction factionB);

bool FactionExists(const Faction& f);


