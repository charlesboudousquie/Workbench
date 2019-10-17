/*!***************************************************************************************
\file       EnemyAIScript.cpp
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "EnemyAIScript.h"
#include "../Components/RigidBody.hpp"
#include "PlayerControllerScript.hpp"
#include <Vector3.hpp>


enemyAIScript::enemyAIScript(typeRT p_data): scriptCPP(p_data)
{
}

void enemyAIScript::onStart()
{
	m_targets = findObjectsByComponent<playerControllerScript>();
	m_myTransform = getComponent<transform>();
	m_myRigidBody = getComponent<rigidBody>();
}

void enemyAIScript::onUpdate()
{
	if(m_ActivationState == false)
	{
		return;
	}
	vector3 l_myPosition = m_myTransform->getPosition();
	vector3 l_targetPosition;
		for(auto i_target : m_targets)
		{
			l_targetPosition =  i_target->getComponent<transform>()->getPosition();

			float l_distanceSquared = (l_targetPosition - l_myPosition).distance();
			if(l_distanceSquared < m_shortestDistance)
			{
				m_shortestDistance = l_distanceSquared;
				m_currentTarget = i_target;
			}
		}

		if(m_currentTarget)
		{
			if((m_shortestDistance < m_atkDistance) && !m_attackState)
			{
				m_currentDirection = vector3(0, 0, 0);
				m_myRigidBody->setVelocity(vector3(0, 0, 0));
				m_attackState = true;
				// Add rotate, facing, attacking, etc.
			}
			if (m_shortestDistance > m_atkDistance && m_attackState)
			{
				m_attackState = false;
				m_currentDirection = l_targetPosition - l_myPosition;
			}
			else
			{
				m_currentDirection = l_targetPosition - l_myPosition;
			}
		}
		else
		{
			m_currentDirection = vector3(0, 0, 0);
		}

		//m_myTransform->setRotation(m_currentDirection);
		if(m_attackState == false)
		{
			m_myRigidBody->applyImpulse((m_currentDirection.normalized()) * m_scaleFactor);

			vector3 l_clampedVel = m_myRigidBody->getVelocity();
			l_clampedVel.x = std::clamp(m_myRigidBody->getVelocity().x, -m_maxSpeed, m_maxSpeed);
			l_clampedVel.y = std::clamp(m_myRigidBody->getVelocity().y, -m_maxSpeed, m_maxSpeed),
			l_clampedVel.z = std::clamp(m_myRigidBody->getVelocity().z, -m_maxSpeed, m_maxSpeed);
			m_myRigidBody->setVelocity(l_clampedVel);
		}
}

bool enemyAIScript::getActivationState()
{
	return m_ActivationState;
}

void enemyAIScript::setActivationState(bool p_state)
{
	m_ActivationState = p_state;
}
