/*!***************************************************************************************
\file       ParticleEmitter.cpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "ParticleEmitter.hpp"
#include <Vector4Serialization.hpp>
#include <Color4Serialization.hpp>

typeRT particleEmitter::toTypeRT() const
{

	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	std::vector<typeRT> members;

	typeRT direction = ::toTypeRT(m_data.m_direction);
	direction.setVariableName("direction");
	members.push_back(direction);

	std::vector<typeRT> colorMinMaxMember;
	colorMinMaxMember.push_back(::toTypeRT(m_data.m_colorMin));
	colorMinMaxMember.push_back(::toTypeRT(m_data.m_colorMax));
	typeRT colorRange = typeRT("colorRange", colorMinMaxMember);
	members.push_back(colorRange);

	std::vector<typeRT> sizeMinMax;
	sizeMinMax.push_back(typeRT("sizeMin",m_data.m_sizeMin));
	sizeMinMax.push_back(typeRT("sizeMax", m_data.m_sizeMax));
	typeRT sizeRange = typeRT("sizeMinMax", sizeMinMax);
	members.push_back(sizeRange);

	std::vector<typeRT> forceMinMax;
	forceMinMax.push_back(typeRT("forceMin", m_data.m_minForce));
	forceMinMax.push_back(typeRT("forceMax", m_data.m_maxForce));
	typeRT forceRange = typeRT("forceMinMax", forceMinMax);
	members.push_back(forceRange);

	std::vector<typeRT> accelerationMinMax;
	accelerationMinMax.push_back(typeRT("accelerationMin", m_data.m_minAcceleration));
	accelerationMinMax.push_back(typeRT("accelerationMax", m_data.m_maxAcceleration));
	typeRT accelerationRange = typeRT("accelrationMinMax", accelerationMinMax);
	members.push_back(accelerationRange);

	std::vector<typeRT> glowMinMax;
	glowMinMax.push_back(typeRT("glowMin", m_data.m_glowMin));
	glowMinMax.push_back(typeRT("glowMax", m_data.m_glowMax));
	typeRT glowRange = typeRT("glowMinMax", glowMinMax);
	members.push_back(glowRange);

	std::vector<typeRT> lifeMinMax;
	lifeMinMax.push_back(typeRT("lifeMin", m_data.m_minLife));
	lifeMinMax.push_back(typeRT("lifeMax", m_data.m_maxLife));
	typeRT lifeRange = typeRT("lifeMinMax", lifeMinMax);
	members.push_back(lifeRange);
		
	typeRT maxParticles("maxParticles", static_cast<int>(m_data.m_maxParticles));
	members.push_back(maxParticles);

	typeRT batchSize("batchSize", static_cast<int>(m_data.m_batchSize));
	members.push_back(batchSize);

	typeRT batchCount("batchCount", static_cast<int>(m_data.m_batchCount));
	members.push_back(batchCount);

	typeRT interval("interval", static_cast<int>(m_data.m_interval));
	members.push_back(interval);

	typeRT on("on", static_cast<bool>(m_data.m_on));
	members.push_back(on);


	for (auto i_member : members)
	{
		result.insertMember(i_member);
	}

  return result;
}

bool particleEmitter::getActiveFlag()
{
	return m_data.m_on;
}

void particleEmitter::setActiveFlag(bool flag)
{
	m_data.m_on = flag;
}

void particleEmitter::updateFromTypeRT(typeRT & p_type)
{
	auto l_members = p_type.members();

	if (l_members.find("direction") != l_members.end())
	{
		m_data.m_direction = ::toVector4(p_type.member("direction"));
	}
	if (l_members.find("colorRange") != l_members.end())
	{
		typeRT colorRange = p_type.member("colorRange");
		std::vector<typeRT> members = colorRange.array();
		m_data.m_colorMin = ::toColor4(members[0]);
		m_data.m_colorMax = ::toColor4(members[1]);
	}
	if (l_members.find("sizeRange") != l_members.end())
	{
		typeRT sizeRange = p_type.member("sizeRange");
		std::vector<typeRT> members = sizeRange.array();
		m_data.m_sizeMin = (members[0].getFloat());
		m_data.m_sizeMax = (members[1]).getFloat();
	}
	if (l_members.find("forceRange") != l_members.end())
	{
		typeRT forceRange = p_type.member("forceRange");
		std::vector<typeRT> members = forceRange.array();
		m_data.m_minForce = (members[0].getFloat());
		m_data.m_maxForce = (members[1]).getFloat();
	}
	if (l_members.find("accelerationRange") != l_members.end())
	{
		typeRT accelerationRange = p_type.member("accelerationRange");
		std::vector<typeRT> members = accelerationRange.array();
		m_data.m_minAcceleration = (members[0].getFloat());
		m_data.m_maxAcceleration = (members[1]).getFloat();
	}
	if (l_members.find("glowRange") != l_members.end())
	{
		typeRT glowRange = p_type.member("glowRange");
		std::vector<typeRT> members = glowRange.array();
		m_data.m_glowMin = (members[0].getFloat());
		m_data.m_glowMax = (members[1]).getFloat();
	}
	if (l_members.find("lifeRange") != l_members.end())
	{
		typeRT lifeRange = p_type.member("lifeRange");
		std::vector<typeRT> members = lifeRange.array();
		m_data.m_minLife = (members[0].getFloat());
		m_data.m_maxLife = (members[1]).getFloat();
	}
	if (l_members.find("maxParticles") != l_members.end())
	{
		m_data.m_maxParticles = p_type.member("maxParticles").getInt();
	}
	if (l_members.find("batchSize") != l_members.end())
	{
		m_data.m_batchSize = p_type.member("batchSize").getInt();
	}
	if (l_members.find("batchCount") != l_members.end())
	{
		m_data.m_batchCount = p_type.member("batchCount").getInt();
	}
	if (l_members.find("interval") != l_members.end())
	{
		m_data.m_interval = p_type.member("interval").getInt();
	}
	if (l_members.find("on") != l_members.end())
	{
		m_data.m_on = p_type.member("on").getBool();
	}
}

particleEmitter::particleEmitter(typeRT& p_type)
{

	m_data.m_direction = ::toVector4(p_type.member("direction"));
	typeRT colorRange = p_type.member("colorRange");
	std::vector<typeRT> members = colorRange.array();
	m_data.m_colorMin = ::toColor4(members[0]);
	m_data.m_colorMax = ::toColor4(members[1]);

	/*typeRT angleRange = p_type.member("angleRange");
	std::vector<typeRT> members = angleRange.array();
	m_data.m_minAcceleration = (members[0].getFloat());
	m_data.m_maxAcceleration = (members[1]).getFloat();*/
	typeRT sizeRange = p_type.member("sizeRange");
	/*std::vector<typeRT> members = sizeRange.array();
	m_data.m_sizeMin = (members[0]).getFloat();
	m_data.m_sizeMax = (members[1].getFloat());*/
	typeRT forceRange = p_type.member("forceRange");
	/*std::vector<typeRT> members = forceRange.array();
	m_data.m_minForce = (members[0]).getFloat();
	m_data.m_maxForce = (members[1].getFloat());*/
	typeRT accelerationRange = p_type.member("accelerationRange");
	/*std::vector<typeRT> members = accelerationRange.array();
	m_data.m_minAcceleration = (members[0]).getFloat();
	m_data.m_maxAcceleration = (members[1].getFloat());*/
	typeRT glowRange = p_type.member("glowRange");
	/*std::vector<typeRT> members = glowRange.array();
	m_data.m_glowMin = (members[0]).getFloat();
	m_data.m_glowMax = (members[1].getFloat());*/
	typeRT lifeRange = p_type.member("lifeRange");
	/*std::vector<typeRT> members = lifeRange.array();
	m_data.m_minLife = (members[0].getFloat());
	m_data.m_maxLife = (members[1].getFloat());*/

	m_data.m_maxParticles = p_type.member("maxParticles").getInt();
	m_data.m_batchSize = p_type.member("batchSize").getInt();
	m_data.m_batchCount = p_type.member("batchCount").getInt();
	m_data.m_interval = p_type.member("interval").getInt();
	m_data.m_on = p_type.member("on").getBool();

}
