/*!***************************************************************************************
\file       SceneEntityReference.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/

#include "SceneEntityReference.hpp"

__declspec(dllexport) sceneEntityReference::sceneEntityReference()
	: m_type{entityType::None}, m_id{0}
{
}

__declspec(dllexport) sceneEntityReference::sceneEntityReference(entityType p_type, unsigned int p_id)
	: m_type{p_type}, m_id{p_id}
{
}

bool __declspec(dllexport) sceneEntityReference::isValid() const
{
	return m_type != entityType::None;
}

bool __declspec(dllexport) sceneEntityReference::isScene() const
{
	return m_type == entityType::Scene;
}

bool __declspec(dllexport) sceneEntityReference::isSpace() const
{
	return m_type == entityType::Space;
}

bool __declspec(dllexport) sceneEntityReference::isObject() const
{
	return m_type == entityType::Object;
}

bool __declspec(dllexport) sceneEntityReference::isComponent() const
{
	return m_type == entityType::Component;
}

sceneEntityReference::entityType __declspec(dllexport) sceneEntityReference::getType() const
{
	return m_type;
}

unsigned int __declspec(dllexport) sceneEntityReference::getID() const
{
	return m_id;
}
