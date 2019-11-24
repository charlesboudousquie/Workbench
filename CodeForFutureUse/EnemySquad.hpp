/*!***************************************************************************************
\file       EnemySquad.hpp
\author     Charles Boudousquie
\date       11/22/2019
\copyright  All content  2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Structure containing enemy units in game.
*****************************************************************************************/
#pragma once
#include "Squad.hpp"

class EnemySquad : public Squad
{
    // only 1 ring for enemy squad
    SquadRingPtr m_ring;

public:

    EnemySquad(space* space_, int maxRings, Faction faction) : Squad(space_, maxRings, faction) {}

    bool joinSquad(objID) override;
    bool Full() const override;

};


