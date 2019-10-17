/*!***************************************************************************************
\file       DynamicWaypointGraph.cpp
\author     Alex Kudebeh
\date       9/10/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Implementation for the dynamic waypoint graph system used for AI pathfinding
*****************************************************************************************/

//======== Self Include ================================================================//
#include "DynamicWaypointGraph.hpp"
//======== 1st Party Includes ==========================================================//
#include "WaypointNode.hpp"
#include "WaypointNodeSet.hpp"
#include "Transform.hpp"
#include "Renderer.hpp"
#include "Engine.hpp"
#include "GameObjectGatherer.hpp"
#include "DebugLines.hpp"
//======== 3rd Party Includes ==========================================================//
#include <Vector3.hpp>
#include <Vector2.hpp>
#include <Reflection.hpp>
#include <GameObjectSerialization.hpp>
//======== Types =======================================================================//

//!<Contains intersection information when attempting to create new connections
struct intersection
{
	intersection(nodeConnection oldCon, nodeConnection newCon, vector3 intersectPoint_)
		: oldConnection(oldCon), newConnection(newCon), intersectPoint(intersectPoint_)
	{
	}

	//!<The already valid connection being tested against
	nodeConnection oldConnection;
	//!<The new connection that is being tested
	nodeConnection newConnection;

	//!<The point of intersection
	vector3 intersectPoint;
};


//======== Defines =====================================================================//
#define DEBUG_LINE_TAG "pathing"

//======== Static Declarations =========================================================//

//======== Constructors & Destructor ===================================================//

//======== Getters & Setters ===========================================================//

//======== Overrides ===================================================================//

//======== Functionality ===============================================================//
void dynamicWaypointGraph::createLevelPath()
{
	//testRuns();

	GameObjectFiltering::componentTypeSet node_pattern;
	node_pattern.setType(waypointNode::getType());
	std::vector<std::shared_ptr<gameObject>> nodes = getSystemManager()->getGameObjectGatherer()->getList(node_pattern);

	//This creates paths between every node in the scene, IGNORING LOS
	for (int i = 0; i < nodes.size(); ++i)
	{
		componentHandle<waypointNode> n = nodes[i]->getComponent<waypointNode>();
		for (int j = i + 1; j < nodes.size(); ++j)
		{
			componentHandle<waypointNode> n2 = nodes[j]->getComponent<waypointNode>();
			nodeConnection newConnection = connectNodes(n, n2);
			validConnections.push_back(newConnection);
			addDebugLine(newConnection);
		}
	}
}

void dynamicWaypointGraph::createNodeSetPaths()
{
	//Get all objects with node sets
	GameObjectFiltering::componentTypeSet nodeSet_pattern;
	nodeSet_pattern.setType(waypointNodeSet::getType());
	std::vector<std::shared_ptr<gameObject>> nodeSets = getSystemManager()->getGameObjectGatherer()->getList(nodeSet_pattern);
	
	for (int i = 0; i < nodeSets.size(); ++i)
	{
		componentHandle<waypointNodeSet> n = nodeSets[i]->getComponent<waypointNodeSet>();

		//Inefficient to make a new node "prefab" every node set. Rework this
		//Make new node
		std::shared_ptr<gameObject> node1 = nodeSets[i]->getSpace()->instantiateGameObject("Node Set Point One");
		componentHandle<renderer> l_renderer_comp = node1->addComponent<renderer>();
		l_renderer_comp->setPrimitiveType(CUBE_FILE);
		l_renderer_comp->setProgramType(programType::enm_forward);
		node1->getComponent<renderer>()->setMaterialName("solidred.mtl");
		node1->getComponent<transform>()->setScale(vector3(.25, .25, .25));
		node1->addComponent<waypointNode>();

		//Prefab made here
		typeRT newNodeRT = toTypeRT(node1);

		//Set node positions
		vector3 offset = nodeSets[i]->getComponent<transform>()->getPosition();
		node1->getComponent<transform>()->setPosition(offset + n->nodeOne);

		std::shared_ptr<gameObject> node2 = nodeSets[i]->getSpace()->instantiateGameObject(newNodeRT);
		node2->getComponent<transform>()->setPosition(offset + n->nodeTwo);
		node2->setName("Node Set Point Two");

		std::shared_ptr<gameObject> node3 = nodeSets[i]->getSpace()->instantiateGameObject(newNodeRT);
		node3->getComponent<transform>()->setPosition(offset + n->nodeThree);
		node3->setName("Node Set Point Three");

		std::shared_ptr<gameObject> node4 = nodeSets[i]->getSpace()->instantiateGameObject(newNodeRT);
		node4->getComponent<transform>()->setPosition(offset + n->nodeFour);
		node4->setName("Node Set Point Four");

		//Update node parents
		nodeSets[i]->addChild(node1);
		nodeSets[i]->addChild(node2);
		nodeSets[i]->addChild(node3);
		nodeSets[i]->addChild(node4);

		//Get node pointers
		componentHandle<waypointNode> nodeOneComponent = node1->getComponent<waypointNode>();
		componentHandle<waypointNode> nodeTwoComponent = node2->getComponent<waypointNode>();
		componentHandle<waypointNode> nodeThreeComponent = node3->getComponent<waypointNode>();
		componentHandle<waypointNode> nodeFourComponent = node4->getComponent<waypointNode>();

		//Add to node set list
		n->nodes.push_back(nodeOneComponent);
		n->nodes.push_back(nodeTwoComponent);
		n->nodes.push_back(nodeThreeComponent);
		n->nodes.push_back(nodeFourComponent);

		//Connect nodes, add debug lines
		connectNodeSet(n);
	}
}

void dynamicWaypointGraph::stitchPath()
{
	GameObjectFiltering::componentTypeSet nodeSet_pattern;
	nodeSet_pattern.setType(waypointNodeSet::getType());
	std::vector<std::shared_ptr<gameObject>> nodeSets = getSystemManager()->getGameObjectGatherer()->getList(nodeSet_pattern);

	for (int i = 0; i < nodeSets.size(); ++i)
	{
		handleNodeSetCreation((nodeSets[i]->getComponent<waypointNodeSet>()));
	}
}

void dynamicWaypointGraph::clear()
{
	//clear waypointNode connections and debug drawing lines
	GameObjectFiltering::componentTypeSet node_pattern;
	node_pattern.setType(waypointNode::getType());
	std::vector<std::shared_ptr<gameObject>> nodes = getSystemManager()->getGameObjectGatherer()->getList(node_pattern);

	std::vector<std::shared_ptr<gameObject>>::iterator nodeIterator = nodes.begin();
	for (nodeIterator; nodeIterator != nodes.end(); ++nodeIterator)
	{
		componentHandle<debugLines> lines = debugLines::getLinesByTag((*nodeIterator), DEBUG_LINE_TAG);
		lines->getLineBuffer().clear();
		(*nodeIterator)->getComponent<waypointNode>()->connections.clear();
	}

	//delete node set node children, and clear their list
	GameObjectFiltering::componentTypeSet nodeSet_pattern;
	nodeSet_pattern.setType(waypointNodeSet::getType());
	std::vector<std::shared_ptr<gameObject>> nodeSets = getSystemManager()->getGameObjectGatherer()->getList(nodeSet_pattern);

	std::vector<std::shared_ptr<gameObject>>::iterator setIterator = nodeSets.begin();
	for (setIterator; setIterator != nodeSets.end(); ++setIterator)
	{
		componentHandle<waypointNodeSet> set = (*setIterator)->getComponent<waypointNodeSet>();

		for (std::list<componentHandle<waypointNode>>::iterator nodeIterator = set->nodes.begin();
			nodeIterator != set->nodes.end();
			++nodeIterator)
		{
			set->getGameObject().lock()->getSpace()->destroyGameObject((*nodeIterator)->getGameObject().lock());
		}
		set->nodes.clear();
	}

	//clear all valid and invalid connections
	validConnections.clear();
	invalidConnections.clear();
}

void dynamicWaypointGraph::testRuns()
{
	//pointIsOnSegment(vector2 x3)
	bool pios1 = pointIsOnSegment(vector2(0, 0), vector2(1, 1), vector2(2, 2)); //false
	bool pios2 = pointIsOnSegment(vector2(1, 1), vector2(0, 0), vector2(2, 2)); //true
	bool pios3 = pointIsOnSegment(vector2(1, 1), vector2(1, 1), vector2(2, 2)); //true

	//getLineOrientation(vector3 x3)
	float glo1 = getLineOrientation(vector3(0, 0, 0), vector3(1, 0, 1), vector3(2, 0, 2)); //0
	float glo2 = getLineOrientation(vector3(0, 0, 0), vector3(1, 0, 1), vector3(1, 0, 2)); //counter clockwise
	float glo3 = getLineOrientation(vector3(0, 0, 0), vector3(1, 0, 1), vector3(1, 0, -1)); //clockwise

	//lineIntersectionPoint(vector2 x4) (assumes lines intersect)
	vector2 lip1 = lineIntersectionPoint(vector2(0, 0), vector2(2, 0), vector2(1, -1), vector2(1, 1)); //(1,0)
	vector2 lip2 = lineIntersectionPoint(vector2(0, 0), vector2(1, 1), vector2(.9, .9), vector2(2, 2)); //(0,0) breaks on colinear
	vector2 lip3 = lineIntersectionPoint(vector2(0, 0), vector2(1, 1), vector2(0, .5), vector2(1, .5)); //(.5,.5)

	
	return;
}


//Stitch together a new set of nodes into the existing graph
void dynamicWaypointGraph::handleNodeSetCreation(componentHandle<waypointNodeSet> newSet)
{
	//Find new intersections for valid paths
	//Foreach valid path
	//Check if new connection in the set of nodes intersects with valid connection
	std::list<intersection> newIntersections = findIntersections(newSet);
	
	//If no intersection found, skip it:
	if (newIntersections.size() == 0)
		return;
	
	//If intersection found:
	std::list<intersection>::iterator it = newIntersections.begin();

	for (it; it != newIntersections.end(); ++it)
	{
		//Case: 2 non-object paths
		//Ignore - intersections here do not matter
		if (it->oldConnection.onObjectParameter == it->newConnection.onObjectParameter &&
			it->oldConnection.onObjectParameter == false)
			continue;

		//Case: 1 open, 1 object
		if (it->oldConnection.onObjectParameter != it->newConnection.onObjectParameter)
		{
			nodeConnection & openConnection = (it->oldConnection.onObjectParameter == false) ? it->oldConnection : it->newConnection;
			nodeConnection & objectConnection = (it->oldConnection.onObjectParameter == true) ? it->oldConnection : it->newConnection;

			//Convert vector3 into vector 2 for line intersections
			vector3 objectVector3 = getConnectionVector(objectConnection);
			vector3 openVector3 = getConnectionVector(openConnection);

			//Rotate object connection that is intersecting -90 degrees (clockwise 90 degrees)
			vector2 objectVector = vector2(objectVector3.z, -objectVector3.x);
			vector2 openVector = vector2(openVector3.x, openVector3.z);

			/* NOTE: It is assumed that node connections surround an object clockwise,
			 * thus -90 degree rotation will point towards the object that causes collision */

			//Dot old connection and newly rotated connection
			float dot = objectVector.dotP(openVector);

			//If <0, possible connection with second -> first from open path
			if (dot < 0)
			{
				//Connect second open path node to new object nodes
				nodeConnection newConnectionOne(openConnection.secondNode, objectConnection.firstNode, false, true);
				nodeConnection newConnectionTwo(openConnection.secondNode, objectConnection.secondNode, false, true);

				addDebugLine(newConnectionOne, vector3(0, 1, 1));
				addDebugLine(newConnectionTwo, vector3(0, 1, 1));

				//Validate new paths
				validateConnection(newConnectionOne);
				validateConnection(newConnectionTwo);
			}
			//If >= 0, possible connection with first -> second from open path
			else
			{
				//Connect first open node to new object nodes 
				nodeConnection newConnectionOne(openConnection.firstNode, objectConnection.firstNode, false, true);
				nodeConnection newConnectionTwo(openConnection.firstNode, objectConnection.secondNode, false, true);

				addDebugLine(newConnectionOne, vector3(0, 1, 1));
				addDebugLine(newConnectionTwo, vector3(0, 1, 1));

				//Valide new paths
				validateConnection(newConnectionOne);
				validateConnection(newConnectionTwo);
			}

			invalidateConnection(openConnection);

			removeDebugLine(openConnection);
		}

		//TODO
		//Case: 2 object paths
		if (it->oldConnection.onObjectParameter == it->newConnection.onObjectParameter &&
			it->oldConnection.onObjectParameter == true)
		{
			//One node will be "inner" node for each path
			//Inner nodes are ones that are inside the opposing object, thus should be disabled
			//If (connectionOne.first - connectionTwo.first) dot (connectionOne.direction.rotated -90 degrees) <= 0
				//connectionTwo.second = disabled - invalidate all connections to node, fix them
			//Else
				//connectionTwo.first = disabled

			//If (connectionTwo.first - connectionOne.first) dot (connectionTwo.direction.rotated -90 degrees) <= 0
				//connectionOne.second = disabled
			//Else
				//connectionOne.first = disabled
		}
	}
}

//TODO
//When node sets gets deleted, need to revalidate old paths
void dynamicWaypointGraph::handleNodeSetDeletion()
{
	
}

//======== Helper Functions ============================================================//

//TODO
//Handle breaking all connections to a node, then stitching connections around it
//NOTE: This is not deleting a node - it will still exist
void dynamicWaypointGraph::disableNode(componentHandle<waypointNode> n)
{
	
}

//NEEDS MORE THROROUGH TESTING
//Potential error case - can this be called with one tovalidate node inside an object?
void dynamicWaypointGraph::validateConnection(nodeConnection toValidate)
{
	//Check new connection against all valid connections
	std::list<intersection> intersections = findIntersections(toValidate);

	//No intersections, all good
	if (intersections.size() == 0)
	{
		validConnections.push_back(toValidate);
		return;
	}

	//If intersection found, create new connection(s) with the closest hit for each node
	bool intersectFound = false;
	float closestDist = std::numeric_limits<float>().max();
	float farthestDist = -1;

	nodeConnection closest = intersections.front().oldConnection;
	nodeConnection farthest = intersections.front().oldConnection;

	//Compare intersection points to this node
	vector3 firstNodePosition = getNodePosition((toValidate.firstNode));

	//Loop through intersections, keeping track of farthest and closest object path intersections only
	std::list<intersection>::iterator it = intersections.begin();
	for (it; it != intersections.end(); ++it)
	{
		//Ignore intersections with open paths
		if (it->oldConnection.onObjectParameter == false)
			continue;

		intersectFound = true;
		float dist = (it->intersectPoint - firstNodePosition).distanceSquared();
		if (dist > farthestDist)
		{
			farthestDist = dist;
			farthest = it->oldConnection;
		}
		if (dist < closestDist)
		{
			closestDist = dist;
			closest = it->oldConnection;
		}
	}

	//Recursive call validate on new connections
	if (intersectFound)
	{
		nodeConnection closestOne(toValidate.firstNode, closest.firstNode, false, true);
		nodeConnection closestTwo(toValidate.firstNode, closest.secondNode, false, true);
		nodeConnection farthestOne(toValidate.secondNode, farthest.firstNode, false, true);
		nodeConnection farthestTwo(toValidate.secondNode, farthest.secondNode, false, true);

		removeDebugLine(toValidate);

		validateConnection(closestOne);
		validateConnection(closestTwo);
		validateConnection(farthestOne);
		validateConnection(farthestTwo);
	}
}

//NEEDS MORE THROROUGH TESTING
//Returns all connections, not just the closest one
std::list<intersection> dynamicWaypointGraph::findIntersections(componentHandle<waypointNodeSet> newSet) const
{
	std::list<intersection> toReturn;
	
	//For each valid connection, check for intersection with new set
	std::list<nodeConnection> setConnections = getConnections(newSet);
	
	std::list<nodeConnection>::iterator i;
	for (i = setConnections.begin(); i != setConnections.end(); ++i)
	{
		//Find all intersections and merge them into toReturn
		std::list<intersection> newIntersections = findIntersections(*i);
		for (std::list<intersection>::iterator j = newIntersections.begin(); j != newIntersections.end(); ++j)
		{
			toReturn.push_back(*j);
		}
	}
	
	return toReturn;
}

//NEEDS MORE THROROUGH TESTING
//Returns all connections, not just the closest one
std::list<intersection> dynamicWaypointGraph::findIntersections(nodeConnection connection) const
{
	std::list<intersection> toReturn;

	std::list<nodeConnection>::const_iterator j;
	for (j = validConnections.begin(); j != validConnections.end(); ++j)
	{
		//If intersection found, add to list
		std::pair<bool, vector3> intersect = doConnectionsIntersect(connection, *j);
		if (intersect.first)
			toReturn.push_back(intersection(*j, connection, intersect.second));
	}

	return toReturn;
}


std::list<nodeConnection> dynamicWaypointGraph::getConnections(componentHandle<waypointNodeSet> set) const
{
	std::list<nodeConnection> toReturn;

	//Assumption: Getting each node's first connection forms a full loop around the object
	for (std::list<componentHandle<waypointNode>>::iterator it = set->nodes.begin(); it != set->nodes.end(); ++it)
	{
		toReturn.push_back((*it)->connections.front());
	}
	
	return toReturn;
}

vector3 dynamicWaypointGraph::getConnectionVector(const nodeConnection & c) const
{
	vector3 firstPos = getNodePosition((c.firstNode));
	vector3 secondPos = getNodePosition((c.secondNode));

	return (secondPos - firstPos);
}

vector3 dynamicWaypointGraph::getNodePosition(componentHandle<waypointNode> n) const
{
	return n->getGameObject().lock()->getComponent<transform>()->getGlobalPosition();
}

//Line intersections:
//https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
std::pair<bool, vector3> dynamicWaypointGraph::doConnectionsIntersect(nodeConnection first, nodeConnection second) const
{
	//If paths share a common node, ignore it
	if (first.firstNode == second.firstNode ||
		first.firstNode == second.secondNode ||
		first.secondNode == second.firstNode ||
		first.secondNode == second.secondNode)
		return std::make_pair<bool, vector3>(false, vector3(0, 0, 0));

	//Given two lines A-B and C-D,
	//If the orientation of ABC and ABD are not equal && CDA and CDB are not equal
	//then no intersection
	
	//Get positions of all nodes
	vector3 pointA = getNodePosition(first.firstNode);
	vector3 pointB = getNodePosition(first.secondNode);
	vector3 pointC = getNodePosition(second.firstNode);
	vector3 pointD = getNodePosition(second.secondNode);

	int ABCOrientation = getLineOrientation(pointA, pointB, pointC);
	int ABDOrientation = getLineOrientation(pointA, pointB, pointD);
	int CDAOrientation = getLineOrientation(pointC, pointD, pointA);
	int CDBOrientation = getLineOrientation(pointC, pointD, pointB);

	//Definite intersect
	if (ABCOrientation != ABDOrientation && CDAOrientation != CDBOrientation)
	{
		vector2 intersectPoint = lineIntersectionPoint( vector2(pointA.x, pointA.z),
														vector2(pointB.x, pointB.z),
														vector2(pointC.x, pointC.z),
														vector2(pointD.x, pointD.z));

		return std::make_pair(true, vector3(intersectPoint.x, intersectPoint.y, 0));
	}

	//Special cases: colinear
	//Lines are colinear if a point on one line lies between the two points on the other line
	//In case of ABC, determine if C is between A and B
	if (ABCOrientation == 0 &&
		pointIsOnSegment(vector2(pointC.x, pointC.z), vector2(pointA.x, pointA.z), vector2(pointB.x, pointB.z)))
	{
		return std::make_pair(true, pointC);
	}
	if (ABDOrientation == 0 &&
		pointIsOnSegment(vector2(pointD.x, pointD.z), vector2(pointA.x, pointA.z), vector2(pointB.x, pointB.z)))
	{
		return std::make_pair(true, pointD);
	}
	if (CDAOrientation == 0 &&
		pointIsOnSegment(vector2(pointA.x, pointA.z), vector2(pointC.x, pointC.z), vector2(pointD.x, pointD.z)))
	{
		return std::make_pair(true, pointA);
	}
	if (CDBOrientation == 0 &&
		pointIsOnSegment(vector2(pointB.x, pointB.z), vector2(pointC.x, pointC.z), vector2(pointD.x, pointD.z)))
	{
		return std::make_pair(true, pointB);
	}

	return std::make_pair(false, vector3());
}

bool dynamicWaypointGraph::pointIsOnSegment(vector2 point, vector2 segmentPointA, vector2 segmentPointB) const
{
	//X needs to be between the segment x's, and Y between segment y's
	if (point.x <= std::max(segmentPointA.x, segmentPointB.x) &&
		point.x >= std::min(segmentPointA.x, segmentPointB.x) &&
		point.y <= std::max(segmentPointA.y, segmentPointB.y) &&
		point.y >= std::min(segmentPointA.y, segmentPointB.y))
		return true;

	return false;
}

//Line orientation:
//https://www.geeksforgeeks.org/orientation-3-ordered-points/
//THIS IS ACTUALLY 2D. REWRITE BEFORE MOVING TO MATH LIBRARY
int dynamicWaypointGraph::getLineOrientation(vector3 a, vector3 b, vector3 c) const
{
	float floatValue = (b.z - a.z) * (c.x - b.x) - (c.z - b.z) * (b.x - a.x);
	if (floatValue > 0)
		return 1;
	if (floatValue < 0)
		return -1;
	return 0;
}
	
//Line intersection point
//https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection
vector2 dynamicWaypointGraph::lineIntersectionPoint(vector2 a, vector2 b, vector2 c, vector2 d) const
{
	float bottom = (a.x - b.x) * (c.y - d.y) - (a.y - b.y) * (c.x - d.x);

	//LINES COLINEAR
	if (bottom == 0)
		return vector2(0, 0);

	float x = ((a.x * b.y - a.y * b.x) * (c.x - d.x) - (a.x - b.x) * (c.x * d.y - c.y * d.x)) / bottom;
	float y = ((a.x * b.y - a.y * b.x) * (c.y - d.y) - (a.y - b.y) * (c.x * d.y - c.y * d.x)) / bottom;

	return vector2(x, y);
}

nodeConnection dynamicWaypointGraph::connectNodes(componentHandle<waypointNode> a, 
												  componentHandle<waypointNode> b, bool isObjectPath, bool isValid)
{
	nodeConnection newConnection = nodeConnection(a, b, isObjectPath, isValid);
	a->connections.push_back(newConnection);
	b->connections.push_back(newConnection);
	return newConnection;
}

void dynamicWaypointGraph::invalidateConnection(nodeConnection nc)
{
	std::list<nodeConnection>::iterator it = validConnections.begin();
	for (it; it != validConnections.end(); ++it)
	{
		if (nc.firstNode == it->firstNode && nc.secondNode == it->secondNode)
		{
			invalidConnections.push_back(nc);
			validConnections.erase(it);
			return;
		}
	}
}

void dynamicWaypointGraph::connectNodeSet(componentHandle<waypointNodeSet> nodeSet)
{
	//This is not optimized
	/* Connects all nodes in a set, making sure the first connection of each node
	 * goes clockwise around the object
	 */
	std::list<componentHandle<waypointNode>>::iterator it = nodeSet->nodes.begin();
	for (it; it != nodeSet->nodes.end(); ++it)
	{
		std::list<componentHandle<waypointNode>>::iterator it2 = it;
		++it2;

		if (it2 == nodeSet->nodes.end())
			it2 = nodeSet->nodes.begin();

		nodeConnection newConnection = nodeConnection(*it, *it2, true, true);
		
		(*it)->connections.push_front(newConnection);
		(*it2)->connections.push_back(newConnection);

		addDebugLine(newConnection, vector3(1, 0, 0));
	}
}

void dynamicWaypointGraph::removeDebugLine(nodeConnection nc)
{
	componentHandle<debugLines> pathLines = debugLines::getLinesByTag(nc.firstNode->getGameObject().lock(), DEBUG_LINE_TAG);
	std::vector<lineSegment> & lines = pathLines->getLineBuffer();

	for (std::vector<lineSegment>::iterator it = lines.begin(); it != lines.end(); ++it)
	{
		if (it->end == getNodePosition((nc.secondNode)))
		{
			lines.erase(it);
			return;
		}
	}
}

//Adds debug line to first node in connection
void dynamicWaypointGraph::addDebugLine(nodeConnection nc, vector3 color)
{
	componentHandle<debugLines> pathLines = debugLines::getLinesByTag(nc.firstNode->getGameObject().lock(), DEBUG_LINE_TAG);
	pathLines->getLineBuffer().push_back(lineSegment(getNodePosition((nc.firstNode)), getNodePosition((nc.secondNode)), color));
}

/*
std::pair<bool, vector3> dynamicWaypointGraph::doConnectionsIntersectTest(vector3 pointA, vector3 pointB, vector3 pointC, vector3 pointD)
{
	int ABCOrientation = getLineOrientation(pointA, pointB, pointC);
	int ABDOrientation = getLineOrientation(pointA, pointB, pointD);
	int CDAOrientation = getLineOrientation(pointC, pointD, pointA);
	int CDBOrientation = getLineOrientation(pointC, pointD, pointB);

	//Definite intersect
	if (ABCOrientation != ABDOrientation && CDAOrientation != CDBOrientation)
	{
		vector2 intersectPoint = lineIntersectionPoint(vector2(pointA.x, pointA.y),
			vector2(pointB.x, pointB.y),
			vector2(pointC.x, pointC.y),
			vector2(pointD.x, pointD.y));

		return std::make_pair(true, vector3(intersectPoint.x, intersectPoint.y, 0));
	}

	//Special cases: colinear
	//Lines are colinear if a point on one line lies between the two points on the other line
	if (ABCOrientation == 0 &&
		pointIsOnSegment(vector2(pointC.x, pointC.y), vector2(pointA.x, pointA.y), vector2(pointB.x, pointB.y)))
	{
		return std::make_pair(true, pointC);
	}
	if (ABDOrientation == 0 &&
		pointIsOnSegment(vector2(pointD.x, pointD.y), vector2(pointA.x, pointA.y), vector2(pointB.x, pointB.y)))
	{
		return std::make_pair(true, pointD);
	}
	if (CDAOrientation == 0 &&
		pointIsOnSegment(vector2(pointA.x, pointA.y), vector2(pointC.x, pointC.y), vector2(pointD.x, pointD.y)))
	{
		return std::make_pair(true, pointA);
	}
	if (CDBOrientation == 0 &&
		pointIsOnSegment(vector2(pointB.x, pointB.y), vector2(pointC.x, pointC.y), vector2(pointD.x, pointD.y)))
	{
		return std::make_pair(true, pointB);
	}

	return std::make_pair(false, vector3());
}
*/
