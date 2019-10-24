/*!***************************************************************************************
\file       SpawnerScript.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "SpawnerScript.h"
#include "EnemyAIScript.h"
#include "../Components/Renderer.hpp"
#include "../Physics/PhysicsSystem.hpp"
#include "GameObjectSerialization.hpp"

spawnerScript::spawnerScript(typeRT p_data): scriptCPP(p_data)
{
}

void spawnerScript::onStart()
{
	auto l_enemy = createGameObjectOnMySpace("spawnedEnemy");
	l_enemy->getComponent<transform>()->setScale(vector3(0.75f, 0.75f, 0.75f));
	l_enemy->addComponent<enemyAIScript>();

	l_enemy->addComponent<rigidBody>();
	l_enemy->getComponent<rigidBody>()->setCollisionLayer(collisionLayer::enm_enemy, collisionLayer::enm_player | collisionLayer::enm_world | collisionLayer::enm_enemy);
	l_enemy->getComponent<rigidBody>()->setMass(20.0f);

	auto l_renderer_comp = l_enemy->addComponent<renderer>();
	l_renderer_comp->setPrimitiveType(CUBE_FILE);
	l_renderer_comp->setProgramType(programType::enm_forward);
	l_enemy->getComponent<renderer>()->setMaterialName("solidred.mtl");

	m_Prefab = ::toTypeRT(l_enemy);
	getMySpace()->destroyGameObject(l_enemy);
}

void spawnerScript::onUpdate()
{
	if(m_spawnTimer <= 0)
	{
		m_spawnTimer = m_spawnRate;
		spawnEnemy(m_spawnCount);
	}
	else
	{
		m_spawnTimer -= dt();
	}
}

void spawnerScript::spawnEnemy(int p_count)
{
	for (int i = 0; i < p_count; ++i)
	{
		auto l_enemy = getMySpace()->instantiateGameObject(m_Prefab);

		vector3 l_myPosition = getComponent<transform>()->getPosition();
		l_enemy->getComponent<transform>()->setPosition(l_myPosition);
		getSystem<physicsSystem>()->addPhysicsObjectToWorld(l_enemy);
	}
}
