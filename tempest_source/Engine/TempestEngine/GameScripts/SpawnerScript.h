/*!***************************************************************************************
\file       SpawnerScript.h
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once
#include "../Components/ScriptCPP.hpp"


class spawnerScript : public scriptCPP
{
public:
	spawnerScript() = default;
	spawnerScript(typeRT p_data);
	componentType const type() const override { return spawnerScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<spawnerScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "spawnerScript", true, true, false); }

	void onStart() override;
	void onUpdate() override;
	void spawnEnemy(int p_count);
private:
	unsigned int m_spawnCount = 1;
	float m_spawnRate = 1.0f;
	float m_spawnTimer = 0.0f;
	typeRT m_Prefab;
};
