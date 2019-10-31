/*!***************************************************************************************
\file       EnemyWithinRange.cpp
\author     Charles Boudousquie
\date       10/30/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Decorator/Conditional that checks if enemy is within a certain predetermined 
            distance.
*****************************************************************************************/
#pragma once
#include "EnemyWithinRange.hpp"

void EnemyWithinRange::Init()
{
  // get our actor
  auto actor = GetActor();
  
  // get our position
  auto ourPos = actor->GetComponent<Transform>()->GetPosition();
  
  // get all enemies and check if one of them is in range
  TODO??????????
  
  
  if(withinRange)
  {
    Decorator::Init();
  }
  else
  {
    GetTask()->SetPhase(BehaviorPhase::Done);
    GetTask()->SetResult(BehaviorResult::FAILURE);
  }
  
}

void EnemyWithinRange::Update()
{
  GetTask()->SetPhase(BehaviorPhase::Done);
}


