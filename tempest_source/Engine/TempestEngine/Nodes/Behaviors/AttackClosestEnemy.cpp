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

constexpr char attackDamageStr[] = "Attack Damage";
constexpr char timeBetweenAttackStr[] = "Time Between Attacks";

const SpecialData AttackClosestEnemy::specialData
{ 
    {SpecialData::Entry{ DATA_TYPE::FLOAT, "Attack Damage", -1.0f},
    SpecialData::Entry{ DATA_TYPE::FLOAT, "Time Between Attacks", -1.0f}} 
};

bool AttackClosestEnemy::EnemyInRange(GameObjectPtr enemy)
{
    auto mypos = getActor()->getComponent<transform>()->getPosition();

    Blackboard& blackboard = GetBlackboard();

    float minimumRange = blackboard.getValue<float>("minimumRange");

    auto theirPos = enemy->getComponent<transform>()->getPosition();

    if ((mypos - theirPos).distance() <= minimumRange)
    {
        return true;
    }

    return false;
}

GameObjectPtr AttackClosestEnemy::GetClosestEnemy()
{
    GameObjectFiltering::componentTypeSet node_pattern;
    node_pattern.setType(Allegiance::getType());

    std::vector<std::shared_ptr<gameObject>> actors = this->getObjectGatherer()->getList(node_pattern);

    auto actor = getActor();

    Allegiance::Faction myFaction = actor->getComponent<Allegiance>()->getFaction();

    GameObjectPtr closestEnemy = nullptr;

    auto myPos = actor->getComponent<transform>()->getPosition();

    float closestDistance = 1000000;

    for (auto otherActor : actors)
    {
        Allegiance::Faction theirFaction = otherActor->getComponent<Allegiance>()->getFaction();
        
        // if confirmed to be enemy
        if (myFaction != theirFaction && theirFaction != Allegiance::Faction::NONE)
        {
            // get their position
            auto enemyPos = otherActor->getComponent<transform>()->getPosition();

            auto distance = (myPos - enemyPos).distance();

            if (distance < closestDistance)
            {
                closestDistance = distance;
                closestEnemy = otherActor;
            }
        }
    }

    return closestEnemy;
}

void AttackClosestEnemy::Init()
{
    Blackboard& blackboard = GetBlackboard();
    blackboard.setValue(timeBetweenAttackStr, this->timeBetweenAttacks);
    Leaf::Init();
}

void AttackClosestEnemy::Update(float dt)
{
  // NOTE: it is assumed that some behavior above this one in the tree has already confirmed
  // that an enemy is actually within range
  GameObjectPtr closest_enemy = GetClosestEnemy();

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

//// returns default render data
//typeRT AttackClosestEnemy::getRenderData()
//{
//    // get default data
//    auto defaultRenderData = Leaf::leafOnRender();
//
//    // add specialized data
//    typeRT l_attack_damage(attackDamageStr, float(-1));
//    defaultRenderData.member("Node Render Data").insertMember(l_attack_damage);
//
//    typeRT l_time_between(timeBetweenAttackStr, float(-1));
//    defaultRenderData.member("Node Render Data").insertMember(l_time_between);
//
//    // return combined typeRT data
//    return defaultRenderData;
//}
//void AttackClosestEnemy::fillSpecialRenderData(const rapidjson::Value &JSON, typeRT& data)
//{
//    assert(data.getVariableName() == "Node Render Data");
//
//    // if json does not have attack damage variable then we are screwed
//    if (JSON.HasMember(attackDamageStr))
//    {
//        // set typert to have max time value
//        data.member(attackDamageStr).setFloat(JSON[attackDamageStr].GetFloat());
//    }
//    else
//    {
//        throw std::runtime_error("Missing attack damage");
//    }
//
//    // if json does not have attack time variable then we are screwed
//    if (JSON.HasMember(timeBetweenAttackStr))
//    {
//        // set typert
//        data.member(timeBetweenAttackStr).setFloat(JSON[timeBetweenAttackStr].GetFloat());
//    }
//    else
//    {
//        throw std::runtime_error("Missing time between attacks");
//    }
//}
//
//// serialization
//void AttackClosestEnemy::serializeSpecialData(typeRT& data, rapidjson::Document& doc)
//{
//    assert(data.getVariableName() == "Node Render Data");
//
//    float attackTime = data.member(timeBetweenAttackStr).getFloat();
//
//    if (doc.HasMember(timeBetweenAttackStr))
//    {
//        doc[timeBetweenAttackStr].SetFloat(attackTime);
//    }
//    else
//    {
//        doc.AddMember(timeBetweenAttackStr, attackTime, doc.GetAllocator());
//    }
//
//
//    float attackDamage = data.member(attackDamageStr).getFloat();
//    if (doc.HasMember(attackDamageStr))
//    {
//        doc[attackDamageStr].SetFloat(attackTime);
//    }
//    else
//    {
//        doc.AddMember(attackDamageStr, attackDamage, doc.GetAllocator());
//    }
//
//}

// when reading from file this function will be called
// to set maxTime
void AttackClosestEnemy::updateFromFile(const rapidjson::Value &value)
{
    this->attackDamage = value[attackDamageStr].GetFloat();
    this->timeBetweenAttacks = value[timeBetweenAttackStr].GetFloat();
}

