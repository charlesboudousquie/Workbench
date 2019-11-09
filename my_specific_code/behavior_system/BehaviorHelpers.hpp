/*!***************************************************************************************
\file       BehaviorHelpers.hpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      Functions that various behaviors can use during gameplay, includes
            finding nearby enemies etc.
*****************************************************************************************/
#pragma once
#include <memory>
#include "GameObject.hpp"
#include "Vector3.hpp"

typedef std::shared_ptr<gameObject> GameObjectPtr;

class gameObjectGatherer;

// check if said character still exists in space
bool NPC_Is_Alive(objID npc, space*);

// B - A
vector3 vecBetween(GameObjectPtr A, GameObjectPtr B);

// get distance between actor and some other npc
float GetDistance(GameObjectPtr actor, GameObjectPtr other);

float GetDistanceSquared(GameObjectPtr actor, GameObjectPtr other);

GameObjectPtr GetClosestEnemy(GameObjectPtr actor, gameObjectGatherer *);

