/*!***************************************************************************************
\file       GoToEnemy.cpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      Pushes npc to the nearest enemy.
*****************************************************************************************/
#include "GoToEnemy.hpp"
#include "BehaviorSystem/BehaviorHelpers.hpp"

const char* minDistStr = "Minimum Distance To Enemy";

const SpecialData GoToEnemy::specialData
{
    {{DATA_TYPE::FLOAT, minDistStr, -1.0f}}
};

void GoToEnemy::updateFromFile(const rapidjson::Value &value)
{
    this->minDistToEnemy = value[minDistStr].GetFloat();
}

void GoToEnemy::Init()
{
   // mark nearest enemy
    Blackboard& blackboard = GetBlackboard();

    auto nearestEnemy = GetClosestEnemy(getActor(), getObjectGatherer());

    blackboard.setValue<GameObjectPtr>("Target", nearestEnemy);

    Leaf::Init();
}


void GoToEnemy::Update(float)
{
    auto target = GetBlackboard().getValue<GameObjectPtr>("Target");
    auto dist = GetDistance(getActor(), target);

    if (dist <= this->minDistToEnemy)
    {
        GetTask()->SetPhase(BehaviorPhase::DONE);
        GetTask()->SetResult(BehaviorResult::SUCCESS);
    }

}




