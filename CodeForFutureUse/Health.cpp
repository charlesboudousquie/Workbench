/*!***************************************************************************************
\file       Health.cpp
\author     Charles Boudousquie
\date       10/30/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Health component for npc or player.
*****************************************************************************************/
#include "Health.hpp"

void Health::DecreaseHealth(int amount)
{
  health-= amount;
}

void Health::IncreaseHealth(int amount)
{
  health += amount;
}   
    
int Health::GetHealth()
{
  return health;
}
    
void Health::SetHealth(int amount)
{
  health = amount;
}

bool Health::IsDead()
{
  return health <= 0;
}
    