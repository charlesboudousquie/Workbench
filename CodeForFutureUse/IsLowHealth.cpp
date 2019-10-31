/*!***************************************************************************************
\file       IsLowHealth.cpp
\author     Charles Boudousquie
\date       10/30/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Decorator/Conditional that checks if npc has low health.
*****************************************************************************************/
#include "IsLowHealth.hpp"

void IsLowHealth::Init()
{
  auto actor = GetActor();
  
  auto health = actor->GetComponent<Health>()->GetHealth();

  // if health is low then give to child
  if(health < 50)
  {
    Decorator::Init();
  }
  // otherwise we are not low on health so return false
  else
  {
    GetTask()->SetPhase(BehaviorPhase::Done);
  }
  
}
    
void IsLowHealth::Update()
{
  // at this point child has given task back to us.
  // Child has already set "result" in task so just exit normally
  GetTask()->SetPhase(BehaviorPhase::Done);
}




