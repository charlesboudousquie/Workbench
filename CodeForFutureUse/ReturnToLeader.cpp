/*!***************************************************************************************
\file       ReturnToLeader.cpp
\author     Charles Boudousquie
\date       10/30/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Leaf node that sends actor back to squad leader.
*****************************************************************************************/
#include "ReturnToLeader.hpp"

ReturnToLeader::Update(float dt)
{
    auto leader = find leader;
    auto leaderPos = leader->GetComponent<transform>()->GetPosition();
    
    auto pos = GetActor()->GetComponent<transform>()->GetPosition();
    
    auto directionToLeader = leaderPos - pos;
    
    float distance = length(directionToLeader);
    
    if(distance < 2)
    {
      GetTask()->SetPhase(BehaviorPhase::Done);
      GetTask()->SetResult(BehaviorResult::SUCCESS);
    }
    else
    {
      auto rigid_Body = GetActor()->GetComponent<rigidBody>();
      // if no rigidbody then something went horribly wrong
      if (rigid_Body.isNullptr())
      {
          std::string message = "Error object: " + actor->getName() + " HAS NO rigidbody";
          throw std::runtime_error(message);
      }
      
      //normalize vector between us and enemy
      
      directionToLeader = normalize(directionToLeader);
      rigid_Body->applyImpulse(directionToLeader);
    }
    
}

