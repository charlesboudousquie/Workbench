/*!***************************************************************************************
\file       PlayerSquad.hpp
\author     Charles Boudousquie
\date       11/22/2019
\copyright  All content  2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Structure containing player's minions in game.
*****************************************************************************************/
#pragma once
#include "Squad.hpp"

class PlayerSquad : public Squad
{

    std::vector<SquadRingPtr> m_rings;

    // get squad ring that contains unit
    SquadRingPtr GetRingFromID(objID unit);

public:

    PlayerSquad(space* space_, int maxRings, Faction faction) : Squad(space_, maxRings, faction) {}

    bool joinSquad(objID) override;
    bool Full() const override;

};

