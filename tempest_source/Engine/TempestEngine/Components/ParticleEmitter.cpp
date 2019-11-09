/*!***************************************************************************************
\file       ParticleEmitter.cpp
\author     Cody Cannell, Seraphina Kim
\date       7/31/18
\copyright  All content 2018-2019 DigiPen (USA) Corporation, all rights reserved.
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
/*
	std::vector<typeRT> colorMinMaxMember;
	colorMinMaxMember.push_back(::toTypeRT(m_data.m_colorMin));
	colorMinMaxMember.push_back(::toTypeRT(m_data.m_colorMax));
	typeRT colorRange = typeRT("colorRange", "colorMinMax",colorMinMaxMember);
	members.push_back(colorRange);*/

	std::vector<typeRT> startColorRange;
	startColorRange.push_back(::toTypeRT(m_data.m_startColRange));
	startColorRange.push_back(::toTypeRT(m_data.m_startColRange2));
	typeRT startColorRanges = typeRT("colorRange", "startColorRange", startColorRange);
	members.push_back(startColorRanges);

	std::vector<typeRT> endColorRange;
	endColorRange.push_back(::toTypeRT(m_data.m_endColRange));
	endColorRange.push_back(::toTypeRT(m_data.m_endColRange2));
	typeRT endColorRanges = typeRT("colorRange", "endColorRange", endColorRange);
	members.push_back(endColorRanges);

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

	typeRT shape("emitterShape", static_cast<int>(m_data.m_shape));
	members.push_back(shape);

	typeRT type("emitterType", static_cast<int>(m_data.m_Type));
	members.push_back(type);


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
	/*if (l_members.find("colorMinMax") != l_members.end())
	{
		typeRT colorRange = p_type.member("colorMinMax");
		std::vector<typeRT> members = colorRange.array();
		m_data.m_colorMin = ::toColor4(members[0]);
		m_data.m_colorMax = ::toColor4(members[1]);
	}*/
	if (l_members.find("startColorRange") != l_members.end())
	{
		typeRT startColorRange = p_type.member("startColorRange");
		std::vector<typeRT> members = startColorRange.array();
		m_data.m_startColRange = ::toColor4(members[0]);
		m_data.m_startColRange2 = ::toColor4(members[1]);
	}
	if (l_members.find("endColorRange") != l_members.end())
	{
		typeRT endColorRange = p_type.member("endColorRange");
		std::vector<typeRT> members = endColorRange.array();
		m_data.m_endColRange = ::toColor4(members[0]);
		m_data.m_endColRange2 = ::toColor4(members[1]);
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
	if (l_members.find("emitterShape") != l_members.end())
	{
		m_data.m_shape = static_cast<emitterShape>(p_type.member("emitterShape").getInt());
	}
	if (l_members.find("emitterType") != l_members.end())
	{
		m_data.m_Type = static_cast<emitterType>(p_type.member("emitterType").getInt());
	}
}

particleEmitter::particleEmitter(typeRT& p_type)
{

	m_data.m_direction = ::toVector4(p_type.member("direction"));
	/*typeRT colorRange = p_type.member("colorMinMax");
	std::vector<typeRT> members = colorRange.array();
	m_data.m_colorMin = ::toColor4(members[0]);
	m_data.m_colorMax = ::toColor4(members[1]);*/

	typeRT startColorRange = p_type.member("startColorRange");
	std::vector<typeRT> startmembers = startColorRange.array();
	m_data.m_startColRange = ::toColor4(startmembers[0]);
	m_data.m_startColRange2 = ::toColor4(startmembers[1]);

	typeRT endColorRange = p_type.member("endColorRange");
	std::vector<typeRT> endmembers = endColorRange.array();
	m_data.m_endColRange = ::toColor4(endmembers[0]);
	m_data.m_endColRange2 = ::toColor4(endmembers[1]);


	typeRT sizeRange = p_type.member("sizeRange");
	typeRT forceRange = p_type.member("forceRange");
	typeRT accelerationRange = p_type.member("accelerationRange");
	typeRT glowRange = p_type.member("glowRange");
	typeRT lifeRange = p_type.member("lifeRange");

	m_data.m_maxParticles = p_type.member("maxParticles").getInt();
	m_data.m_batchSize = p_type.member("batchSize").getInt();
	m_data.m_batchCount = p_type.member("batchCount").getInt();
	m_data.m_interval = p_type.member("interval").getInt();
	m_data.m_on = p_type.member("on").getBool();

	m_data.m_shape = static_cast<emitterShape>(p_type.member("emitterShape").getInt());
	m_data.m_Type = static_cast<emitterType>(p_type.member("emitterType").getInt());
}
