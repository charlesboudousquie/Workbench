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
#include "RigidBody.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"

const char* minDistStr = "Minimum Distance To Enemy";
const char* speedStr = "Max Speed";

const SpecialData GoToEnemy::specialData
{
    {
        {DATA_TYPE::FLOAT, minDistStr, -1.0f},
{DATA_TYPE::FLOAT, speedStr, -1.0f}
    }
};

void GoToEnemy::updateFromFile(const rapidjson::Value &value)
{
    this->minDistToEnemy = value[minDistStr].GetFloat();
    auto it = value.FindMember(speedStr);
    this->maxSpeed = value[speedStr].GetFloat();
}

void GoToEnemy::Init()
{
    // mark nearest enemy
    Blackboard& blackboard = GetBlackboard();

    auto nearestEnemy = GetClosestEnemy(getActor(), getObjectGatherer());

    blackboard.setValue<objID>("Target", nearestEnemy->getID());

    Leaf::Init();
}


void GoToEnemy::Update(float dt)
{
    auto currSpace = this->getCurrentSpace();

    Blackboard& blackboard = GetBlackboard();
    auto target = blackboard.getValue<objID>("Target");

    if (NPC_Is_Alive(target, currSpace))
    {
        auto targetObj = currSpace->getGameObjectByID(target);
        assert(targetObj != getActor());
        auto dist = GetDistance(getActor(), targetObj);

        const vector3& theirPos = targetObj->getComponent<transform>()->getPosition();
        const vector3& ourPos = getActor()->getComponent<transform>()->getPosition();

        auto vec_Between = ourPos - theirPos; /// ?????? hOW ??? TODO
        vec_Between = vec_Between.normalize();

        targetObj->getComponent<rigidBody>()->applyImpulse(vec_Between * maxSpeed * dt);

        if (dist <= this->minDistToEnemy)
        {
            GetTask()->SetPhase(BehaviorPhase::DONE);
            GetTask()->SetResult(BehaviorResult::SUCCESS);
        }
    }
    else
    {
        GetTask()->SetPhase(BehaviorPhase::DONE);
        GetTask()->SetResult(BehaviorResult::FAILURE);
    }
}




