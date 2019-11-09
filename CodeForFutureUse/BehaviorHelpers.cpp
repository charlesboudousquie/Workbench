/*!***************************************************************************************
\file       BehaviorHelpers.cpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      Functions that various behaviors can use during gameplay, includes
            finding nearby enemies etc.
*****************************************************************************************/
#include "BehaviorHelpers.hpp"

GameObjectPtr AttackClosestEnemy::GetClosestEnemy(GameObjectPtr actor, gameObjectGatherer *GO_gatherer)
{
    GameObjectFiltering::componentTypeSet node_pattern;
    node_pattern.setType(Allegiance::getType());

    std::vector<std::shared_ptr<gameObject>> actors = GO_gatherer->getList(node_pattern);

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
