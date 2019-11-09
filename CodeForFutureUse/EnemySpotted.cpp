/*!***************************************************************************************
\file       EnemySpotted.cpp
\author     Charles Boudousquie
\date       11/4/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Decorator that tells you if an enemy is near the npc.
*****************************************************************************************/
#include "EnemySpotted.hpp"

void EnemySpotted::Init()
{
    GameObjectFiltering::componentTypeSet node_pattern;
    node_pattern.setType(Allegiance::getType());

    std::vector<std::shared_ptr<gameObject>> actors = this->getObjectGatherer()->getList(node_pattern);

    auto actor = getActor();

    Allegiance::Faction myFaction = actor->getComponent<Allegiance>()->getFaction();

    GameObjectPtr closestEnemy = nullptr;

    auto myPos = actor->getComponent<transform>()->getPosition();

    for (auto otherActor : actors)
    {
        Allegiance::Faction theirFaction = otherActor->getComponent<Allegiance>()->getFaction();
        
        // if confirmed to be enemy
        if (myFaction != theirFaction && theirFaction != Allegiance::Faction::NONE)
        {
            // get their position
            auto enemyPos = otherActor->getComponent<transform>()->getPosition();

            auto distance = (myPos - enemyPos).distance();

            if (distance < spottingDistance)
            {
              // enemy spotted, we are done
              Decorator::Init();
              break;
            }
        }
    }

    // no enemy found
    GetTask()->SetPhase(BehaviorPhase::DONE);
    GetTask()->SetResult(BehaviorResult::FAILURE);
}




void EnemySpotted::Update(float dt)
{
  // assumed that child has finished by this point
  GetTask()->SetPhase(BehaviorPhase::DONE);
}





















