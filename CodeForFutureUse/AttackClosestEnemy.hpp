/*!***************************************************************************************
\file       AttackClosestEnemy.hpp
\author     Charles Boudousquie
\date       11/4/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Leaf behavior that tells npc to attack closest enemy.
*****************************************************************************************/
#pragma once
#include "Leaf.hpp"

class AttackClosestEnemy : public Leaf
{
  public:
  
    void Update(float) override;
  
};


