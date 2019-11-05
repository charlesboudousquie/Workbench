/*!***************************************************************************************
\file       AttackClosestEnemy.cpp
\author     Charles Boudousquie
\date       11/4/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Leaf behavior that tells npc to attack closest enemy.
*****************************************************************************************/
#include "AttackClosestEnemy.hpp"

void AttackClosestEnemy::Update(float dt)
{
  
  auto closest_enemy = GetClosestEnemy();
  
  // decrement health of closest enemy
  closest_enemy->decrement_health();
  
  auto enemyHealth = closest_enemy->GetComponent<Health>();
  
  // if enemy died then we have succeeded
  if(enemyHealth->IsDead())
  {
    auto task = GetTask();
    
    task->SetPhase(BehaviorPhase::Done);
    task->SetResult(BehaviorResult::SUCCESS);
  }
  
}


