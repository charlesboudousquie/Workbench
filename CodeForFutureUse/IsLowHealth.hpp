/*!***************************************************************************************
\file       IsLowHealth.hpp
\author     Charles Boudousquie
\date       10/30/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Decorator/Conditional that checks if npc has low health.
*****************************************************************************************/
#pragma once
#include "Decorator.hpp"

class IsLowHealth : public Decorator
{
  public:
  
    void Init()override;
    
    void Update() override;


};



