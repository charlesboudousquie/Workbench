/*!***************************************************************************************
\file       IsDead.cpp
\author     Charles Boudousquie
\date       10/30/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Decorator/Conditional that checks if npc is dead.
*****************************************************************************************/
#include "IsDead.hpp"
#include "Health.hpp"

void IsDead::Init()
{
  // get actor
  auto actor = getActor();
  
  auto isDead = actor->getComponent<Health>()->IsDead();
  
  // if dead then send task to child behavior
  if(isDead)
  {
    Decorator::Init();
  }
  else
  {
    GetTask()->SetPhase(BehaviorPhase::DONE);
    GetTask()->SetResult(BehaviorResult::FAILURE);
  }

}

void IsDead::Update(float dt)
{
  // child has already set result so just exit this behavior
  GetTask()->SetPhase(BehaviorPhase::DONE);
}



