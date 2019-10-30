/*!***************************************************************************************
\file       DynamicWaypointGraph.cpp
\author     Alex Kudebeh
\date       9/10/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Implementation for the dynamic waypoint graph system used for AI pathfinding
*****************************************************************************************/

/*
Notes to future Alex:
Clean up Create node set. It's ugly
Think about pulling stuff out. This is getting bloated
*/

//======== Self Include ================================================================//
#include "DynamicWaypointGraph.hpp"
//======== 1st Party Includes ==========================================================//
#include "Transform.hpp"
#include "Renderer.hpp"
#include "Engine.hpp"
#include "GameObjectGatherer.hpp"
#include "DebugLines.hpp"
#include "TestingScript.hpp"
//======== 3rd Party Includes ==========================================================//
#include <Vector3.hpp>
#include <Vector2.hpp>
#include <random>
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

	bool operator== (const intersection & other)
	{
		return (this->oldConnection == other.oldConnection && this->newConnection == other.newConnection);
	}

	//!<The point of intersection
	vector3 intersectPoint;
};

//======== Defines =====================================================================//
#define DEBUG_LINE_TAG "pathing"

//======== Static Declarations =========================================================//

//======== Constructors & Destructor ===================================================//

//======== Getters & Setters ===========================================================//

//======== Overrides ===================================================================//
bool nodeConnection::operator==(nodeConnection other) const
{
	if ((this->firstNode == other.firstNode && this->secondNode == other.secondNode) ||
		(this->firstNode == other.secondNode && this->secondNode == other.firstNode))
		return true;
	return false;
}

//======== Functionality ===============================================================//
std::list<vector3> dynamicWaypointGraph::getPath(std::shared_ptr<gameObject> startObject,
												std::shared_ptr<gameObject> endObject)
{
	//create waypointNodes for start/end
	std::shared_ptr<gameObject> start = makeNodeGameobject(startObject->getSpace(), "darkgrey.mtl");
	start->detatchFromParent();
	std::shared_ptr<gameObject> end = start->cloneObject();

	start->setName("Start Node");
	end->setName("End Node");

	start->getComponent<transform>()->setPosition(startObject->getComponent<transform>()->getGlobalPosition());
	end->getComponent<transform>()->setPosition(endObject->getComponent<transform>()->getGlobalPosition());
	
	//"Create level path" with just the two nodes
	nodeConnection newConnection = connectNodes(start->getComponent<waypointNode>(), end->getComponent<waypointNode>());
	validConnections.push_back(newConnection);
	validateConnection(newConnection);

	//A* start to finish
	//delete start/end nodes (unnecessary now)
	//remember to remove nodeConnections to them as well
	//return path

	/* NOTES
	void buildSubGraph(WaypointNode s, WaypointNode e, subGraph & g)

	find intersections with s-e
	if no intersections found
		connect s-e, throw in subGraph
		return
	if intersections found
		Attempt to connect based on intersection points
		recurse on attempted connections
	

	--

	current works because it's node set by node set
	path fails because checks all and only fixes closest
	*/
	return std::list<vector3>(0);
}

void dynamicWaypointGraph::createLevelPath()
{
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
		createNodeSet(n);
	}
}

void dynamicWaypointGraph::stitchPath()
{
	clear();
	createLevelPath();

	GameObjectFiltering::componentTypeSet nodeSet_pattern;
	nodeSet_pattern.setType(waypointNodeSet::getType());
	std::vector<std::shared_ptr<gameObject>> nodeSets = getSystemManager()->getGameObjectGatherer()->getList(nodeSet_pattern);

	for (int i = 0; i < nodeSets.size(); ++i)
	{
		createNodeSet(nodeSets[i]->getComponent<waypointNodeSet>());
		handleNodeSetCreation(nodeSets[i]->getComponent<waypointNodeSet>());
	}
}

void dynamicWaypointGraph::clear()
{
	//clear waypointNode connections and debug drawing lines
	GameObjectFiltering::componentTypeSet node_pattern;
	node_pattern.setType(waypointNode::getType());
	std::vector<std::shared_ptr<gameObject>> nodes = getSystemManager()->getGameObjectGatherer()->getList(node_pattern);

	std::vector<std::shared_ptr<gameObject>>::iterator nodeIterator;
	for (nodeIterator = nodes.begin(); nodeIterator != nodes.end(); ++nodeIterator)
	{
		componentHandle<debugLines> lines = debugLines::getLinesByTag((*nodeIterator), DEBUG_LINE_TAG);
		lines->getLineBuffer().clear();
		(*nodeIterator)->getComponent<waypointNode>()->connections.clear();
	}

	//delete node set node children, and clear their list
	GameObjectFiltering::componentTypeSet nodeSet_pattern;
	nodeSet_pattern.setType(waypointNodeSet::getType());
	std::vector<std::shared_ptr<gameObject>> nodeSets = getSystemManager()->getGameObjectGatherer()->getList(nodeSet_pattern);

	std::vector<std::shared_ptr<gameObject>>::iterator setIterator;
	for (setIterator = nodeSets.begin(); setIterator != nodeSets.end(); ++setIterator)
	{
		componentHandle<waypointNodeSet> set = (*setIterator)->getComponent<waypointNodeSet>();

		for (std::vector<componentHandle<waypointNode>>::iterator nodeIterator = set->nodes.begin();
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

void dynamicWaypointGraph::deleteRandomNodeSet()
{
	GameObjectFiltering::componentTypeSet nodeSet_pattern;
	nodeSet_pattern.setType(waypointNodeSet::getType());
	std::vector<std::shared_ptr<gameObject>> nodeSets = getSystemManager()->getGameObjectGatherer()->getList(nodeSet_pattern);

	std::random_device generator;
	int rand = generator() % nodeSets.size();

	componentHandle<waypointNodeSet> toDelete = nodeSets[rand]->getComponent<waypointNodeSet>();

	handleNodeSetDeletion(toDelete);

	for (std::vector<componentHandle<waypointNode>>::iterator nodeIterator = toDelete->nodes.begin();
		nodeIterator != toDelete->nodes.end();
		++nodeIterator)
	{
		nodeSets[rand]->getSpace()->destroyGameObject((*nodeIterator)->getGameObject().lock());
	}

	toDelete->nodes.clear();
}

void dynamicWaypointGraph::testFunction()
{
	GameObjectFiltering::componentTypeSet script_pattern;
	script_pattern.setType(testingScript::getType());
	std::vector<std::shared_ptr<gameObject>> objects = getSystemManager()->getGameObjectGatherer()->getList(script_pattern);

	if (objects.size() < 2)
	{
		return;
	}
	else
	{
		getPath(objects[0], objects[1]);
	}

}

//TODO: object/object overlap
void dynamicWaypointGraph::handleNodeSetCreation(componentHandle<waypointNodeSet> newSet)
{
	//Find new intersections for valid paths
	//Foreach valid path
	//Check if new connection in the set of nodes intersects with valid connection
	std::list<intersection> newIntersections = findIntersections(newSet);

	std::string setDebugName = newSet->debugName;

	//If no intersection found, skip it:
	if (newIntersections.size() == 0)
		return;
	
	//If intersection found:
	std::list<intersection>::iterator it;

	for (it = newIntersections.begin(); it != newIntersections.end(); ++it)
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
				nodeConnection newConnectionOne = connectNodes(openConnection.secondNode, objectConnection.firstNode, false, true);
				nodeConnection newConnectionTwo = connectNodes(openConnection.secondNode, objectConnection.secondNode, false, true);

				//Validate new paths
				validateConnection(newConnectionOne);
				validateConnection(newConnectionTwo);
			}
			//If >= 0, possible connection with first -> second from open path
			else
			{
				//Connect first open node to new object nodes 
				nodeConnection newConnectionOne = connectNodes(openConnection.firstNode, objectConnection.firstNode, false, true);
				nodeConnection newConnectionTwo = connectNodes(openConnection.firstNode, objectConnection.secondNode, false, true);

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

//testing for invalid connections can be faster by storing valid or not in the node itself
void dynamicWaypointGraph::handleNodeSetDeletion(componentHandle<waypointNodeSet> set)
{
	//Get list of all nodes to be deleted
	std::vector<componentHandle<waypointNode>> deletedNodes = set->nodes;
	//list of all nodes that were connected to a deleted node
	std::list<componentHandle<waypointNode>> detachedNodes;

	for (int i = 0; i < deletedNodes.size(); ++i)
	{
		componentHandle<debugLines> dLines = debugLines::getLinesByTag(deletedNodes[i]->getGameObject().lock(), DEBUG_LINE_TAG);
		std::vector<lineSegment> & lines = dLines->getLineBuffer();
		lines.clear();
	}

	//Add all nodes to detached list that were connected, but are no longer
	//Delete connections, and remove debug lines
	std::list<nodeConnection>::iterator it = validConnections.begin();
	while (it != validConnections.end())
	{
		componentHandle<waypointNode> n1 = it->firstNode;
		componentHandle<waypointNode> n2 = it->secondNode;
		bool match1 = false;
		bool match2 = false;

		//Check if a deleted node is in the connection
		for (int i = 0; i < deletedNodes.size(); ++i)
		{
			if (deletedNodes[i] == n1)
				match1 = true;
			else if (deletedNodes[i] == n2)
				match2 = true;
		}

		//Double match - don't remove debug lines; they are removed already
		if (match1 && match2)
		{
			std::list<nodeConnection>::iterator toDelete = it;
			++it;
			validConnections.erase(toDelete);
		}
		//Single match - remove debug line on opposing (just in case)
		else if (match1 || match2)
		{
			std::list<nodeConnection>::iterator toDelete = it;
			++it;
			//If second node is being deleted, first node must have debug line
			if (match2)
			{
				detachedNodes.push_back(n1);
				removeDebugLine(*toDelete);
			}
			else //match1
			{
				detachedNodes.push_back(n2);
			}
			validConnections.erase(toDelete);
		}
		//No deleted nodes, ignore it
		else
		{
			++it;
		}
	}

	//delete invalid connections that are connected
	it = invalidConnections.begin();
	while (it != invalidConnections.end())
	{
		componentHandle<waypointNode> n1 = it->firstNode;
		componentHandle<waypointNode> n2 = it->secondNode;
		bool deleteThis = false;

		//Check if a deleted node is in the connection
		for (int i = 0; i < deletedNodes.size(); ++i)
		{
			if (deletedNodes[i] == n1 || deletedNodes[i] == n2)
			{
				deleteThis = true;
				break;
			}
		}
		if (deleteThis)
		{
			std::list<nodeConnection>::iterator toDelete = it;
			++it;
			invalidConnections.erase(toDelete);
		}
		else
		{
			++it;
		}
	}
	
	//No duplicates
	detachedNodes.sort();
	detachedNodes.unique();

	std::list<nodeConnection> revalidate;

	//foreach invalid connection
	std::list<nodeConnection>::iterator invalidCheck = invalidConnections.begin(); 
	while(invalidCheck != invalidConnections.end())
	{
		//check if one of it's nodes are among detached
		std::list<componentHandle<waypointNode>>::iterator detachedCheck;
		bool incremented = false;
		for (detachedCheck = detachedNodes.begin(); detachedCheck != detachedNodes.end(); ++detachedCheck)
		{
			if (invalidCheck->firstNode == *detachedCheck || invalidCheck->secondNode == *detachedCheck)
			{
				//if so, remove from invalid connections, and revalidate it
				std::list<nodeConnection>::iterator toDelete = invalidCheck;
				++invalidCheck;
				incremented = true;
				revalidate.push_back(*toDelete);
				invalidConnections.erase(toDelete);
				break;
			}
		}
		if (!incremented)
			++invalidCheck;
	}

	for (invalidCheck = revalidate.begin(); invalidCheck != revalidate.end(); ++invalidCheck)
	{
		validConnections.push_back(*invalidCheck);
		/*
		Required to be in validConnections before validating
		This is because validateConnection may invalidate it if intersections are found,
		which checks for it in validConnection list before adding to invalid
		This was done to prevent duplicate connections in invalidConnections

		Alternative fix - find how duplicates are being added to invalid connections,
		and fix that
		*/
		validateConnection(*invalidCheck);
	}
}

//======== Helper Functions ============================================================//
void dynamicWaypointGraph::createNodeSet(componentHandle<waypointNodeSet> set)
{
	//Make new node
	std::shared_ptr<gameObject> setObject = set->getGameObject().lock();
	std::shared_ptr<gameObject> node1 = makeNodeGameobject(setObject->getSpace());


	//Prefab made here
	std::shared_ptr<gameObject> node2 = node1->cloneObject();
	std::shared_ptr<gameObject> node3 = node1->cloneObject();
	std::shared_ptr<gameObject> node4 = node1->cloneObject();

	//Set node positions
	vector3 offset = setObject->getComponent<transform>()->getPosition();
	vector3 forward = setObject->getComponent<transform>()->getForward();
	vector3 right = setObject->getComponent<transform>()->getRight();

	//noe 1 pos & debugName
	vector3 nodePos = offset + set->nodeOne.x * right + set->nodeOne.z * forward;
	node1->getComponent<transform>()->setPosition(nodePos);
	node1->setName("Node Set Point One");
	node1->getComponent<waypointNode>()->debugName = std::string(set->debugName + " 1");

	//node 2
	nodePos = offset + set->nodeTwo.x * right + set->nodeTwo.z * forward;
	node2->getComponent<transform>()->setPosition(nodePos);
	node2->setName("Node Set Point Two");
	node2->getComponent<waypointNode>()->debugName = std::string(set->debugName + " 2");

	//node 3
	nodePos = offset + set->nodeThree.x * right + set->nodeThree.z * forward;
	node3->getComponent<transform>()->setPosition(nodePos);
	node3->setName("Node Set Point Three");
	node3->getComponent<waypointNode>()->debugName = std::string(set->debugName + " 3");

	//node 4
	nodePos = offset + set->nodeFour.x * right + set->nodeFour.z * forward;
	node4->getComponent<transform>()->setPosition(nodePos);
	node4->setName("Node Set Point Four");
	node4->getComponent<waypointNode>()->debugName = std::string(set->debugName + " 4");

	//Update node parents
	setObject->addChild(node1);
	setObject->addChild(node2);
	setObject->addChild(node3);
	setObject->addChild(node4);

	//Get node pointers
	componentHandle<waypointNode> nodeOneComponent = node1->getComponent<waypointNode>();
	componentHandle<waypointNode> nodeTwoComponent = node2->getComponent<waypointNode>();
	componentHandle<waypointNode> nodeThreeComponent = node3->getComponent<waypointNode>();
	componentHandle<waypointNode> nodeFourComponent = node4->getComponent<waypointNode>();

	//Add to node set list
	set->nodes.push_back(nodeOneComponent);
	set->nodes.push_back(nodeTwoComponent);
	set->nodes.push_back(nodeThreeComponent);
	set->nodes.push_back(nodeFourComponent);

	//Connect nodes, add debug lines
	connectNodeSet(set);
}

std::shared_ptr<gameObject> dynamicWaypointGraph::makeNodeGameobject(space * s, std::string materialName) const
{
	std::shared_ptr<gameObject> toReturn = s->instantiateGameObject("Waypoint Graph Node");
	componentHandle<renderer> l_renderer_comp = toReturn->addComponent<renderer>();
	l_renderer_comp->setPrimitiveType(CUBE_FILE);
	l_renderer_comp->setProgramType(programType::enm_forward);
	toReturn->getComponent<renderer>()->setMaterialName(materialName);
	toReturn->getComponent<transform>()->setScale(vector3(.25, .25, .25));
	toReturn->addComponent<waypointNode>();

	return toReturn;
}

bool dynamicWaypointGraph::nodesAreConnected(componentHandle<waypointNode> n1, componentHandle<waypointNode> n2)
{
	for (int i = 0; i < n1->connections.size(); ++i)
	{
		if (n1->connections[i].first == n2)
			return true;
	}
	return false;
}

//TODO
//Handle breaking all connections to a node, then stitching connections around it
//NOTE: This is not deleting a node - it will still exist
void dynamicWaypointGraph::disableNode(waypointNode n)
{
	
}

void dynamicWaypointGraph::validateConnection(nodeConnection & toValidate)
{
	//Check new connection against all valid connections
	std::list<intersection> intersections = findIntersections(toValidate);

	//No intersections, all good
	if (intersections.size() == 0)
	{
		validConnections.push_back(toValidate);
		addDebugLine(toValidate, { 0,1,1 });
		return;
	}
	//Edge case when a path hits only one wall
	//Try to connect that wall to outside node instead
	if (intersections.size() == 1)
	{
		vector3 otherWallDir = getConnectionVector(intersections.front().oldConnection);
		vector3 myDir = getConnectionVector(toValidate);
		vector2 rotatedOther = vector2(-otherWallDir.z, otherWallDir.x);
		float dot = rotatedOther.dotP(vector2(myDir.x, myDir.z));
		if (dot >= 0)
		{
			attemptConnectionAndValidation(toValidate.secondNode, intersections.front().oldConnection.firstNode);
			attemptConnectionAndValidation(toValidate.secondNode, intersections.front().oldConnection.secondNode);
		}
		else
		{
			attemptConnectionAndValidation(toValidate.firstNode, intersections.front().oldConnection.firstNode);
			attemptConnectionAndValidation(toValidate.firstNode, intersections.front().oldConnection.secondNode);
		}
		return;
	}

	//If intersection found, create new connection(s) with the closest hit for each node
	bool intersectFound = false;
	float closestDist = std::numeric_limits<float>().max();
	float farthestDist = -1;

	nodeConnection closest = intersections.front().oldConnection;
	nodeConnection farthest = intersections.front().oldConnection;

	//Compare intersection points to this node
	vector3 firstNodePosition = getNodePosition(toValidate.firstNode);

	//Loop through intersections, keeping track of farthest and closest object path intersections only
	std::list<intersection>::iterator it;
	for (it = intersections.begin(); it != intersections.end(); ++it)
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
		invalidateConnection(toValidate);
		removeDebugLine(toValidate);

		attemptConnectionAndValidation(toValidate.firstNode, closest.firstNode);
		attemptConnectionAndValidation(toValidate.firstNode, closest.secondNode);

		attemptConnectionAndValidation(toValidate.secondNode, farthest.firstNode);
		attemptConnectionAndValidation(toValidate.secondNode, farthest.secondNode);
	}
}

void dynamicWaypointGraph::attemptConnectionAndValidation(componentHandle<waypointNode> n1, componentHandle<waypointNode> n2)
{
	if (nodesAreConnected(n1, n2))
		return;

	nodeConnection newConnection = connectNodes(n1, n2, false, true);
	validateConnection(newConnection);
}

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

//Returns all connections, not just the closest one
std::list<intersection> dynamicWaypointGraph::findIntersections(nodeConnection connection) const
{
	std::list<intersection> toReturn;

	std::list<nodeConnection>::const_iterator j;
	for (j = validConnections.begin(); j != validConnections.end(); ++j)
	{
		if (connection.onObjectParameter == false && j->onObjectParameter == false)
			continue;

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

	for (int i = 0; i < set->nodes.size(); ++i)
	{
		int next = i + 1;
		if (next == set->nodes.size())
			next = 0;

		bool found = false;
		//Search valid connections
		for (std::list<nodeConnection>::const_iterator it = validConnections.begin(); it != validConnections.end(); ++it)
		{
			//Find connections such that you have a loop around the object
			if (it->firstNode == set->nodes[i] && it->secondNode == set->nodes[next])
			{
				toReturn.push_back(*it);
				found = true;
				break;
			}
		}
		if (found)
			continue;

		//if not found in valid, search invalid
		for (std::list<nodeConnection>::const_iterator it = invalidConnections.begin(); it != invalidConnections.end(); ++it)
		{
			//Find connections such that you have a loop around the object
			if (it->firstNode == set->nodes[i] && it->secondNode == set->nodes[next])
			{
				toReturn.push_back(*it);
				break;
			}
		}
	}
	
	return toReturn;
}

vector3 dynamicWaypointGraph::getConnectionVector(const nodeConnection & c) const
{
	vector3 firstPos = getNodePosition(c.firstNode);
	vector3 secondPos = getNodePosition(c.secondNode);

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
	if ((first.firstNode == second.firstNode) ||
		(first.firstNode == second.secondNode) ||
		(first.secondNode == second.firstNode) ||
		(first.secondNode == second.secondNode))
		return std::make_pair<bool, vector3>(false, vector3(0, 0, 0));

	//Given two lines A-B and C-D,
	//If the orientation of ABC and ABD are not equal && CDA and CDB are not equal
	//then no intersection

	std::vector<std::string> debugNames = { first.firstNode->debugName,
		first.secondNode->debugName,
		second.firstNode->debugName,
		second.secondNode->debugName
	};
	
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

		return std::make_pair(true, vector3(intersectPoint.x, 0, intersectPoint.y));
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

nodeConnection dynamicWaypointGraph::connectNodes(componentHandle<waypointNode> a, componentHandle<waypointNode> b, bool isObjectPath, bool isValid)
{
	nodeConnection newConnection = nodeConnection(a, b, isObjectPath, isValid);

	vector3 diffVector = getNodePosition(a) - getNodePosition(b);
	diffVector.y = 0;
	float cost = diffVector.distanceSquared();

	a->connections.push_back(std::make_pair(b, cost));
	b->connections.push_back(std::make_pair(a, cost));
	return newConnection;
}

void dynamicWaypointGraph::invalidateConnection(nodeConnection & nc)
{
	//If it was valid, erase it
	std::list<nodeConnection>::iterator it;
	for (it = validConnections.begin(); it != validConnections.end(); ++it)
	{
		if (nc == *it)
		{
			invalidConnections.push_back(nc);
			validConnections.erase(it);
			break;
		}
	}

	std::vector<std::pair<componentHandle<waypointNode>, float>>::iterator conWalker;
	//clear connection from first node
	for (conWalker = nc.firstNode->connections.begin(); conWalker != nc.firstNode->connections.end(); ++conWalker)
	{
		if (conWalker->first == nc.secondNode)
		{
			nc.firstNode->connections.erase(conWalker);
			break;
		}
	}
	//clear connection from second node
	for (conWalker = nc.secondNode->connections.begin(); conWalker != nc.secondNode->connections.end(); ++conWalker)
	{
		if (conWalker->first == nc.firstNode)
		{
			nc.secondNode->connections.erase(conWalker);
			break;
		}
	}
}

void dynamicWaypointGraph::connectNodeSet(componentHandle<waypointNodeSet> nodeSet)
{
	std::vector<componentHandle<waypointNode>>::iterator it;
	for (it = nodeSet->nodes.begin(); it != nodeSet->nodes.end(); ++it)
	{
		std::vector<componentHandle<waypointNode>>::iterator it2 = it;
		++it2;

		if (it2 == nodeSet->nodes.end())
			it2 = nodeSet->nodes.begin();

		nodeConnection newConnection = connectNodes(*it, *it2, true, true);
		validConnections.push_back(newConnection);

	    addDebugLine(newConnection, vector3(1, 0, 0));
	}
}

void dynamicWaypointGraph::deleteConnection(const nodeConnection & c)
{
	validConnections.remove(c);
	invalidConnections.remove(c);

	componentHandle<waypointNode> node = c.firstNode;

	//Tell the nodes they are no longer connected
	std::vector<std::pair<componentHandle<waypointNode>, float>>::iterator it;
	for (it = node->connections.begin(); it != node->connections.end(); ++it)
	{
		if (it->first == c.secondNode)
		{
			node->connections.erase(it);
			break;
		}
	}
	node = c.secondNode;
	for (it = node->connections.begin(); it != node->connections.end(); ++it)
	{
		if (it->first == c.firstNode)
		{
			node->connections.erase(it);
			break;
		}
	}
}

void dynamicWaypointGraph::removeDebugLine(nodeConnection nc)
{
	componentHandle<debugLines> pathLines = debugLines::getLinesByTag(nc.firstNode->getGameObject().lock(), DEBUG_LINE_TAG);
	std::vector<lineSegment> & lines = pathLines->getLineBuffer();

	for (std::vector<lineSegment>::iterator it = lines.begin(); it != lines.end(); ++it)
	{
		if (it->end == getNodePosition(nc.secondNode))
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
	pathLines->getLineBuffer().push_back(lineSegment(getNodePosition(nc.firstNode), getNodePosition(nc.secondNode), color));
}
