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

GameObjectPtr AttackClosestEnemy::GetClosestEnemy(GameObjectPtr actor, gameObjectGatherer *);

