/*!***************************************************************************************
\file       WaypointNode.hpp
\author     Alex Kudebeh
\date       9/16/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Component for the waypoint node in the graph pathfinding system
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "Component.hpp"
//======== 3rd Party Includes ==========================================================//
#include <list>
#include <IComponentRepository.hpp>
//======== Types =======================================================================//
class waypointNode; //nodeConnection needs this :(

/*!***************************************************************************************
\par struct: nodeConnection
\brief   A connection to another node
*****************************************************************************************/
struct nodeConnection
{
	/*!***************************************************************************************
	\brief  default constructor
	*****************************************************************************************/
	nodeConnection() : firstNode(), secondNode(), onObjectParameter(false), isValid(true)
	{
	}

	// IF YOU ARE CONFUSED ABOUT WHY THIS EXISTS ASK COSEO
	// it wouldn't compile until i added this for some reason
	nodeConnection(const nodeConnection& other) {
		firstNode = other.firstNode;
		secondNode = other.secondNode;
		onObjectParameter = other.onObjectParameter;
		isValid = other.isValid;
	}

	/*!***************************************************************************************
	\brief  constructor
	\param firstNode_ - The first node in the connection
	\param secondNode_ - The first node in the connection
	\param onObjectParameter_ - If the connection is on an object parameter
	\param isValid_ - If the path is being blocked or not
	*****************************************************************************************/
	nodeConnection(componentHandle<waypointNode> firstNode_, componentHandle<waypointNode> secondNode_, bool onObjectParameter_, bool isValid_) :
		firstNode(firstNode_), secondNode(secondNode_), onObjectParameter(onObjectParameter_), isValid(isValid_)
	{
	}

	//!<First node in the connection
	componentHandle<waypointNode> firstNode;
	//!<Second node in the connection
	componentHandle<waypointNode> secondNode;
	//!<If this object is on an object parameter, or an open path
	bool onObjectParameter;
	//!<If this connections is still valid (invalid means object is blocking it)
	bool isValid;
};

//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//
class nodeSet;

/*!***************************************************************************************
\par class: waypointNode
\brief   The waypointNode class is used with the dynamic waypoint graph pathfinding system
*****************************************************************************************/
class waypointNode : public componentCRTP<waypointNode>
{
	public:
		//!<The connections to other nodes
		std::list<nodeConnection> connections;

		/*!***************************************************************************************
		\brief  componentType getter
		\return The type of component this is
		*****************************************************************************************/
		componentType const type() const { static componentType const s_type = componentType::forConcrete<waypointNode>(); return s_type; }

		/*!***************************************************************************************
		\brief  static componentType getter
		\return The type of component this is
		*****************************************************************************************/
		static componentType const getType() { static componentType const s_type = componentType::forAbstract<waypointNode>(); return s_type; }

		/*!***************************************************************************************
		\brief  static componentTypeDetails getter
		\return Getter for the type's details
		*****************************************************************************************/
		static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Waypoint Node", true, true, false); }

		void updateFromTypeRT(typeRT & p_type);
		typeRT toTypeRT() const;
};
// namespace <namespace name>

