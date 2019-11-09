/*!***************************************************************************************
\file       MoveTowardsEnemy.hpp
\author     Charles Boudousquie
\date       11/4/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This behavior steers object towards some marked enemy. NOTE:
            Enemy should be "marked" before calling this function.
*****************************************************************************************/
#pragma once
#include "Leaf.hpp"

class MoveTowardsEnemy : public Leaf
{
  vec3 enemyPos;
  float minimumDistanceToEnemy
  
  public:
    void Init() override;
    void Update(float) override;
  
};



