/*!***************************************************************************************
\file       SceneEntityReference.hpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once

class sceneEntityReference final
{
public:
	enum class entityType { Scene = 1, Space = 2, Object = 3, Component = 4, None = 0 };

	__declspec(dllexport) sceneEntityReference();
	__declspec(dllexport) sceneEntityReference(entityType p_type, unsigned int p_id);

	bool __declspec(dllexport) isValid() const;
	bool __declspec(dllexport) isScene() const;
	bool __declspec(dllexport) isSpace() const;
	bool __declspec(dllexport) isObject() const;
	bool __declspec(dllexport) isComponent() const;
	entityType __declspec(dllexport) getType() const;
	unsigned int __declspec(dllexport) getID() const;

private:
	entityType m_type;
	unsigned int m_id;
};

