/*!***************************************************************************************
\file       EnemyWithinRange.hpp
\author     Charles Boudousquie
\date       10/30/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Decorator/Conditional that checks if enemy is within a certain predetermined 
            distance.
*****************************************************************************************/
#pragma once
#include "Decorator.hpp"

class EnemyWithinRange : public Decorator
{
  
  public:
    void Init() override;
    
    void Update() override;

};



