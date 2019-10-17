/*!***************************************************************************************
\file       WaypointNodeSet.hpp
\author     Alex Kudebeh
\date       9/24/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "Component.hpp"
#include "Vector3.hpp"
//======== 3rd Party Includes ==========================================================//
#include <list>
#include <IComponentRepository.hpp>
#include "WaypointNode.hpp" // necessary for component handle reasons. 

//======== Types =======================================================================//

//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

/*!***************************************************************************************
\par class: waypointNodeSet
\brief   The waypointNode class is used with the dynamic waypoint graph pathfinding system
*****************************************************************************************/
class waypointNodeSet : public componentCRTP<waypointNodeSet>
{
public:
	vector3 nodeOne;
	vector3 nodeTwo;
	vector3 nodeThree;
	vector3 nodeFour;

	std::list<componentHandle<waypointNode>> nodes;

	/*!***************************************************************************************
	\brief  componentType getter
	\return The type of component this is
	*****************************************************************************************/
	componentType const type() const { static componentType const s_type = componentType::forConcrete<waypointNodeSet>(); return s_type; }

	/*!***************************************************************************************
	\brief  static componentType getter
	\return The type of component this is
	*****************************************************************************************/
	static componentType const getType() { static componentType const s_type = componentType::forAbstract<waypointNodeSet>(); return s_type; }

	/*!***************************************************************************************
	\brief  static componentTypeDetails getter
	\return Getter for the type's details
	*****************************************************************************************/
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Waypoint Node Set", true, true, false); }

	void updateFromTypeRT(typeRT & p_type);
	typeRT toTypeRT() const;
};
// namespace <namespace name>

