/*!***************************************************************************************
\file       Health.hpp
\author     Charles Boudousquie
\date       10/30/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Health component for npc or player.
*****************************************************************************************/
#pragma once
#include "Component.hpp"
#include "QueryableInterface.hpp"

class Health : public componentCRTP<Agent>, public queryableInterface
{
  int health;
  
  public:
  
    void DecreaseHealth(int amount);
    void IncreaseHealth(int amount);
    
    int GetHealth();
    int SetHealth();
    
    bool IsDead();


};

