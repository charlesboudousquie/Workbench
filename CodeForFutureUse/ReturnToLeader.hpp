/*!***************************************************************************************
\file       ReturnToLeader.hpp
\author     Charles Boudousquie
\date       10/30/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Leaf node that sends actor back to squad leader.
*****************************************************************************************/
#pragma once
#include "Decorator.hpp"

class ReturnToLeader : public Decorator
{
  
  public:
  
    void Update(float) override;
  
};

