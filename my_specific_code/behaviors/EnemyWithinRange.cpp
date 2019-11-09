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
#include "Transform.hpp"
#include "Allegiance.hpp"
#include "GameObjectFilter.hpp"

const char minRangeStr[] = "minimumRange";

const SpecialData EnemyWithinRange::specialData =
{
    {SpecialData::Entry{DATA_TYPE::FLOAT, "minimumRange", -1.0f}}
};

std::vector<GameObjectPtr> EnemyWithinRange::GetEnemyObjects()
{
    GameObjectFiltering::componentTypeSet node_pattern;
    node_pattern.setType(Allegiance::getType());

    std::vector<std::shared_ptr<gameObject>> actors = getObjectGatherer()->getList(node_pattern);

    auto actor = getActor();
    Allegiance::Faction myFaction = actor->getComponent<Allegiance>()->getFaction();

    std::vector<GameObjectPtr> enemies; 

    for (auto otherActor : actors)
    {
        Allegiance::Faction theirFaction = otherActor->getComponent<Allegiance>()->getFaction();
        
        // if they aint us then we attack
        if (theirFaction != myFaction && theirFaction != Allegiance::Faction::NONE)
        {
            enemies.push_back(otherActor);
        }
    }

    return enemies;
}

void EnemyWithinRange::Init()
{
  // get our actor
  auto actor = getActor();
  
  // get our position
  auto ourPos = actor->getComponent<transform>()->getPosition();
  
  // get all enemies and check if one of them is in range
  std::vector<GameObjectPtr> enemyobjects = std::move(GetEnemyObjects());
  
  bool enemyWithinRange = false;

  for (auto enemy : enemyobjects)
  {
      auto& enemyPos = enemy->getComponent<transform>()->getPosition();

      float distance = (ourPos - enemyPos).distance();

      // TODO: optimization idea, compare squared distance instead of euclidean distance,
      // actual distance will be saved normally, any distance user types inside NGE is saved as is.
      // EX: user types in 4, this->minimumRange = 4;
      // minimumRange is saved to file as 4
      // when comparing : minDistance* minDistance <= minimumRange

      if (distance < minimumRange)
      {
          enemyWithinRange = true;
          break;
      }
  }
  
  // if closest enemy distance is within range
  if(enemyWithinRange)
  {
    Decorator::Init();
  }
  else
  {
    GetTask()->SetPhase(BehaviorPhase::DONE);
    GetTask()->SetResult(BehaviorResult::FAILURE);
  }
  
}

void EnemyWithinRange::Update(float)
{
  GetTask()->SetPhase(BehaviorPhase::DONE);
}

//typeRT EnemyWithinRange::getRenderData()
//{
//    // get default data
//    auto defaultRenderData = Decorator::decoratorOnRender();
//
//    // add specialized data
//    typeRT l_minimum_range(minRangeStr, float(-1));
//    defaultRenderData.member("Node Render Data").insertMember(l_minimum_range);
//
//    // return combined typeRT data
//    return defaultRenderData;
//}
//
//void EnemyWithinRange::fillSpecialRenderData(const rapidjson::Value &JSON, typeRT& data)
//{
//    assert(data.getVariableName() == "Node Render Data");
//
//    // if json does not have minimum range variable then we are screwed
//    if (JSON.HasMember(minRangeStr))
//    {
//        // set typert to have minimum range value
//        data.member(minRangeStr).setFloat(JSON[minRangeStr].GetFloat());
//    }
//    else
//    {
//        throw std::runtime_error("Missing Minimum range");
//    }
//}
//
//// serialization
//void EnemyWithinRange::serializeSpecialData(typeRT& data, rapidjson::Document& doc)
//{
//    assert(data.getVariableName() == "Node Render Data");
//
//    float minRange = data.member(minRangeStr).getFloat();
//
//    if (doc.HasMember(minRangeStr))
//    {
//        doc[minRangeStr].SetFloat(minRange);
//    }
//    else
//    {
//        doc.AddMember(minRangeStr, minRange, doc.GetAllocator());
//    }
//}

// when reading from file this function will be called
// to set maxTime
void EnemyWithinRange::updateFromFile(const rapidjson::Value &value)
{
    this->minimumRange = value[minRangeStr].GetFloat();
}