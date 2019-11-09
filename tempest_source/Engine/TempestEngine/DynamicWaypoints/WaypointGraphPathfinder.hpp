/*!***************************************************************************************
\file       WaypointGraphPathfinder.hpp
\author     Alex Kudebeh
\date       10/11/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Header file for pathfinding with a waypoint graph
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "SystemBase.hpp"
#include "DynamicWaypointGraph.hpp"
//======== 3rd Party Includes ==========================================================//
#include <Vector3.hpp>
#include <list>
//======== Types =======================================================================//
/*!***************************************************************************************
\par class: GraphPath
\brief   This is a stub class to be used as a template / example for class formating.
*****************************************************************************************/
struct graphPath
{
public:
	//!<The path in world coordinates
	std::list<vector3> points;

	/*!***************************************************************************************
	\brief   default constructor, does nothing
	*****************************************************************************************/
	graphPath() = default;

	/*!***************************************************************************************
	\brief   If the path is no longer valid, returns true. Call this before pathing!
	*****************************************************************************************/
	bool isBad() const;

private:
	//!<If the path is bad or good
	bool bad = false;
	//!<The connections used by this path. Held so it can be invalidated if needed
	std::list<nodeConnection> usedConnections;

	friend class waypointGraphPathfinder;
};

//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

/*!***************************************************************************************
\par class: WaypointGraphPathfinder
\brief   Class for path finding using the dynamic waypoint graph
*****************************************************************************************/
class waypointGraphPathfinder : public systemBase
{
public:
	/*!***************************************************************************************
	\brief  Name getter for this system
	\return The name of this system
	*****************************************************************************************/
	const std::string& name() const { static const std::string n("waypointGraphPathfindSystem"); return n; };

	/*!***************************************************************************************
	\brief  Static name getter for this system
	\return The name of this system
	*****************************************************************************************/
	static const std::string& getName() { static const std::string n("waypointGraphPathfindSystem"); return n; }

	/*!***************************************************************************************
	\brief  Creates a path using the dynamic waypoint graph
	\param startPoint - The starting position of the path request
	\param endPoint - The target position of the path request
	\return A shared pointer to the path
	*****************************************************************************************/
	std::shared_ptr<graphPath> getPath(vector3 startPoint, vector3 endPoint);

	/*!***************************************************************************************
	\brief  Alerts graph paths that 1 or more node connections were deleted
	\param invalidated - A list of 1 or more invalidated connections
	*****************************************************************************************/
	void nodeConnectionsInvalidated(std::list<nodeConnection> invalidated);

protected:

private:
	//!<List of valid graph paths created
	std::list<std::shared_ptr<graphPath>> paths;
};
