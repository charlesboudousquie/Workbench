 /*!***************************************************************************************
\file       EnemySquad.cpp
\author     Charles Boudousquie
\date       11/22/2019
\copyright  All content  2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Structure containing enemy units in game.
*****************************************************************************************/
#include "EnemySquad.hpp"

bool EnemySquad::joinSquad(objID unit)
{
    if(this->Full())
    {
        return false;
    }
    
    m_ring->Insert(unit);
    
    return true;
}

bool EnemySquad::Full() const
{
    return m_ring.Full();
}