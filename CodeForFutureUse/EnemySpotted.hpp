/*!***************************************************************************************
\file       EnemySpotted.hpp
\author     Charles Boudousquie
\date       11/4/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Decorator that tells you if an enemy is near the npc.
*****************************************************************************************/
#pragma once
#include "Decorator.hpp"

class EnemySpotted : public Decorator
{
  public:
  
    void Init() override;
    void Update(float) override;
  
};
