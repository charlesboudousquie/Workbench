/*!***************************************************************************************
\file       AttackClosestEnemy.cpp
\author     Charles Boudousquie
\date       11/4/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Leaf behavior that tells npc to attack closest enemy.
*****************************************************************************************/
#include "AttackClosestEnemy.hpp"
#include "Health.hpp"
#include "GameObjectFilter.hpp"
#include "Allegiance.hpp"
#include "SystemBase.hpp"
#include "Transform.hpp"

#include "BehaviorSystem/BehaviorHelpers.hpp"

constexpr char attackDamageStr[] = "Attack Damage";
constexpr char timeBetweenAttackStr[] = "Time Between Attacks";
constexpr char attackRangeStr[] = "Attack Range";

const SpecialData AttackClosestEnemy::specialData
{ 
    {
        SpecialData::Entry{ DATA_TYPE::FLOAT, "Attack Damage", -1.0f},
        SpecialData::Entry{ DATA_TYPE::FLOAT, "Time Between Attacks", -1.0f},
        SpecialData::Entry{DATA_TYPE::FLOAT, "Attack Range", -1.0f}
    }
};

bool AttackClosestEnemy::EnemyInRange(GameObjectPtr enemy)
{
    auto mypos = getActor()->getComponent<transform>()->getPosition();

    Blackboard& blackboard = GetBlackboard();

    //float attackRange = blackboard.getValue<float>(attackRangeStr);

    auto theirPos = enemy->getComponent<transform>()->getPosition();

    if ((mypos - theirPos).distance() <= attackRange)
    {
        return true;
    }

    return false;
}

void AttackClosestEnemy::Init()
{
    Blackboard& blackboard = GetBlackboard();
    blackboard.setValue(timeBetweenAttackStr, this->timeBetweenAttacks);
    blackboard.setValue(attackDamageStr, this->attackDamage);
    Leaf::Init();
}

void AttackClosestEnemy::Update(float dt)
{
#ifdef DEBUGGING_NODES
    printDebug(DEBUG_MESSAGE_TYPE::UPDATE);
#endif

  // NOTE: it is assumed that some behavior above this one in the tree has already confirmed
  // that an enemy is actually within range
  GameObjectPtr closest_enemy = GetClosestEnemy(getActor(), getObjectGatherer());

  auto task = GetTask();

  Blackboard& blackboard = GetBlackboard();
  float attackTime = blackboard.getValue<float>(timeBetweenAttackStr);

  // if an enemy is still nearby
  if (closest_enemy != nullptr && EnemyInRange(closest_enemy))
  {
      attackTime -= dt;

      auto enemyHealth = closest_enemy->getComponent<Health>();
      // if ready to attack
      if (attackTime <= 0)
      {
          // decrement health of closest enemy
          enemyHealth->DecreaseHealth(30); // todo change this

          attackTime = this->timeBetweenAttacks;
      }

      blackboard.setValue<float>(timeBetweenAttackStr, attackTime);


      // if enemy died then we have succeeded
      if (enemyHealth->IsDead())
      {
          task->SetPhase(BehaviorPhase::DONE);
          task->SetResult(BehaviorResult::SUCCESS);
      }
  }
  else
  {
      task->SetPhase(BehaviorPhase::DONE);
      task->SetResult(BehaviorResult::FAILURE);
  }
  
}

// when reading from file this function will be called
// to set maxTime
void AttackClosestEnemy::updateFromFile(const rapidjson::Value &value)
{
    this->attackDamage = value[attackDamageStr].GetFloat();
    this->timeBetweenAttacks = value[timeBetweenAttackStr].GetFloat();
    if (value.HasMember(attackRangeStr))
    {
        this->attackRange = value[attackRangeStr].GetFloat();
    }

}

