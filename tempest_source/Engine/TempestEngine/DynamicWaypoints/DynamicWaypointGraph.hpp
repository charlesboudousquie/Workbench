/*!***************************************************************************************
\file       DynamicWaypointGraph.hpp
\author     Alex Kudebeh
\date       9/10/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Header for Dynamic Waypoint Graphs used for AI pathfinding
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "../SystemBase.hpp"
#include "WaypointNode.hpp" //nodeConnection, waypointNode - needed for valid/invalid connection lists
#include "WaypointNodeSet.hpp"
//======== 3rd Party Includes ==========================================================//
#include <list>
#include <Vector3.hpp>
#include <IComponentRepository.hpp>
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//
struct intersection;
class vector2;

/*!***************************************************************************************
\par class: DynamicWaypointGraph
\brief   This class acts as a container for waypoint nodes and path management
*****************************************************************************************/
class dynamicWaypointGraph : public systemBase
{
	public:
		/*!***************************************************************************************
		\brief  Name getter for this system
		\return The name of this system
		*****************************************************************************************/
		const std::string& name() const { return "dynamicWaypointGraphSystem"; };

		/*!***************************************************************************************
		\brief  Static name getter for this system
		\return The name of this system
		*****************************************************************************************/
		static const std::string& getName() { static const std::string n("dynamicWaypointGraphSystem"); return n; }

		/*!***************************************************************************************
		\brief  Handles stitching the waypoint graph back together when a node set is deleted
		*****************************************************************************************/
		void handleNodeSetDeletion();

		/*!***************************************************************************************
		\brief  Handles connecting a set of new nodes into the current graph
		*****************************************************************************************/
		void handleNodeSetCreation(componentHandle<waypointNodeSet> newSet);

		//TEST FUNCTIONS - will be rewritten/reworked later
		void createLevelPath();
		void createNodeSetPaths();
		void stitchPath();
		void clear();

	protected:

	private:
		/*!***************************************************************************************
		\brief  Handles disabling all connections with a given node
		\param n - The node to invalidate connections to
		*****************************************************************************************/
		void disableNode(componentHandle<waypointNode> n);
		
		/*!***************************************************************************************
		\brief  Compares a new connection with all other valid connections, creating new paths on intersection
		\param newConnection - The new connection to validate against all valid connections
		*****************************************************************************************/
		void validateConnection(nodeConnection newConnection);
		
		/*!***************************************************************************************
		\brief  Gets a list of connections from a node set (no duplicates)
		\param set - The node set to get connections from
		\return The list of all unique connections
		*****************************************************************************************/
		std::list<nodeConnection> getConnections(componentHandle<waypointNodeSet> set) const;
        
		/*!***************************************************************************************
		\brief  Checks if two connections intersect
		\param first - The first connection to check
        \param second - The second connection to check
		\return If the two connections intersect or not, and the intersect position if true
		*****************************************************************************************/
        std::pair<bool, vector3> doConnectionsIntersect(nodeConnection first, nodeConnection second) const;
		//Abstract test version of above function
		//std::pair<bool, vector3> doConnectionsIntersectTest(vector3 pointA, vector3 pointB, vector3 pointC, vector3 pointD);
		
		/*!***************************************************************************************
		\brief  Finds intersections of a given node set with open valid connections
		\param newSet - The node set to get intersections for
		\return The list of all intersections with the node set
		*****************************************************************************************/
		std::list<intersection> findIntersections(componentHandle<waypointNodeSet> newSet) const;

		/*!***************************************************************************************
		\brief  Finds intersections of a given node set with open valid connections
		\param connection - The connection to find intersections for
		\return The list of all intersections with the connection
		*****************************************************************************************/
		std::list<intersection> findIntersections(nodeConnection connection) const;

		/*!***************************************************************************************
		\brief  Gets the directional vector of a connection (first to second)
		\param c - The connection to get the direction of
		\return The direction vector for the connection
		*****************************************************************************************/
		vector3 getConnectionVector(const nodeConnection & c) const;

		/*!***************************************************************************************
		\brief  Gets the world space position of the node
		\param n - The node to get the position of
		\return The node's world space position
		*****************************************************************************************/
		vector3 getNodePosition(componentHandle<waypointNode> n) const;

		/*!***************************************************************************************
		\brief  Returns the orientation of three points. Used for line intersection tests
		\param a - The first point
		\param b - The second point
		\param c - The third point
		\return The orientation of the nodes (1 is clockwise, -1 counter clockwise, 0 colinear)
		*****************************************************************************************/
		int getLineOrientation(vector3 a, vector3 b, vector3 c) const;

		/*!***************************************************************************************
		\brief  Finds the intersection point of two lines
		\param a - Line 1, point 1
		\param b - Line 1, point 2
		\param c - Line 2, point 1
		\param d - Line 2, point 2
		\return The intersection point
		*****************************************************************************************/
		vector2 lineIntersectionPoint(vector2 a, vector2 b, vector2 c, vector2 d) const;
		
		/*!***************************************************************************************
		\brief  Determines if a point is on a given 
		\param point - The point to check
		\param segmentPointA - The first point on a segment
		\param segmentPointB - The second point on the segment
		\return If the point lies on the segment or not
		*****************************************************************************************/
		bool pointIsOnSegment(vector2 point, vector2 segmentPointA, vector2 segmentPointB) const;

		/*!***************************************************************************************
		\brief  Creates a new connection between two nodes. Does NOT add it to valid connections
		\param a - The first node to connect
		\param b - The second node to connect
		\param isObjectPath - If this object is 
		\return The new node connection
		*****************************************************************************************/
		nodeConnection connectNodes(componentHandle<waypointNode> a, 
								    componentHandle<waypointNode> b, bool isObjectPath = false, bool isValid = true);

		/*!***************************************************************************************
		\brief  Moves a node connection from valid to invalid
		\param nc - The node connection to invalidate
		*****************************************************************************************/
		void invalidateConnection(nodeConnection nc);

		void connectNodeSet(componentHandle<waypointNodeSet> nodeSet);

		void addDebugLine(nodeConnection nc, vector3 color = {1,1,1});

		void removeDebugLine(nodeConnection nc);

		//Test function
		void testRuns();

		//!<List of valid connections for path finding
		std::list<nodeConnection> validConnections;
		//!<List of currently invalid connections
		std::list<nodeConnection> invalidConnections;
};
