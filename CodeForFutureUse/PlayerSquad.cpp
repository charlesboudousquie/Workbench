/*!***************************************************************************************
\file       PlayerSquad.cpp
\author     Charles Boudousquie
\date       11/22/2019
\copyright  All content  2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Structure containing player's minions in game.
*****************************************************************************************/
#include "PlayerSquad.hpp"


bool PlayerSquad::joinSquad(objID unit)
{
    // if no room in last ring
    if(rings.back().Full())
    {
        // make a new ring
        SquadRingPtr newSquad = std::make_shared<SquadRing>(....);
        rings.push_back(newSquad);
    }
    
    rings.back()->Insert(unit);
    
    rings.back()->CalculatePositions();
    
    return true; // should this even be returned?
}

bool PlayerSquad::Full() const
{
    std::cout << "Why is this being called?" << std::endl;
    return false;
}

