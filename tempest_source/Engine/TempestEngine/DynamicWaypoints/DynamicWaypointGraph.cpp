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
Helper functions are trying to do too much

Fix invalidate connection. It's doing that dumb thing. You know.

BUGS:
Edge case stitching - need to connect the one it can see (Shouldn't it already?)
-possibly do the expansion stitching (connect neighbors)
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
#include "WaypointGraphPathfinder.hpp"
#include "WaypointNode.hpp"
//======== 3rd Party Includes ==========================================================//
#include <Vector3.hpp>
#include <Vector2.hpp>
#include <random>
//======== Types =======================================================================//

/*!***************************************************************************************
\par struct: intersection
\brief   Holds information regarding two connections intersecting
*****************************************************************************************/
struct intersection
{
	/*!***************************************************************************************
	\brief  intersection constructor
	\param oldCon - The connection that was being tested against
	\param newCon - The connection that was looking for intersections
	\param intersectionPoint_ - The point of intersection in world space
	*****************************************************************************************/
	intersection(nodeConnection oldCon, nodeConnection newCon, vector3 intersectPoint_)
		: oldConnection(oldCon), newConnection(newCon), intersectPoint(intersectPoint_)
	{
	}

	//!<The already valid connection being tested against
	nodeConnection oldConnection;
	//!<The new connection that is being tested
	nodeConnection newConnection;

	/*!***************************************************************************************
	\brief  comparison operator
	\param other - The intersection to compare against
	\return True if old and new connections match
	*****************************************************************************************/
	bool operator== (const intersection & other)
	{
		return (this->oldConnection == other.oldConnection && this->newConnection == other.newConnection);
	}

	//!<The point of intersection
	vector3 intersectPoint;
};

//======== Defines =====================================================================//
#define DEBUG_LINE_TAG "graph"
#define PATH_LINE_TAG "pathing"

//======== Static Declarations =========================================================//

//======== Constructors & Destructor ===================================================//
nodeConnection::nodeConnection(componentHandle<waypointNode> firstNode_, componentHandle<waypointNode> secondNode_, bool onObjectParameter_, bool isValid_) :
	firstNode(firstNode_), secondNode(secondNode_), onObjectParameter(onObjectParameter_), isValid(isValid_)
{
	name = firstNode->debugName + " to " + secondNode->debugName;
}

connectedNode::connectedNode(componentHandle<waypointNode> node_, float cost_, bool sharesObjectParameter_)
	: node(node_), cost(cost_), sharesObjectParameter(sharesObjectParameter_)
{}

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
			nodeConnection newConnection = connectNodesWithConnection(n, n2);
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
	GameObjectFiltering::componentTypeSet testScriptPattern;
	testScriptPattern.setType(testingScript::getType());
	std::vector<std::shared_ptr<gameObject>> objects = getSystemManager()->getGameObjectGatherer()->getList(testScriptPattern);

	if (objects.size() < 2)
	{
		return;
	}
	else
	{
		systemBase * sys = getSystemManager()->getSystem("waypointGraphPathfindSystem");
		if (sys != nullptr)
		{
			waypointGraphPathfinder * wgpf = dynamic_cast<waypointGraphPathfinder *>(sys);
			vector3 pos1 = objects[0]->getComponent<transform>()->getGlobalPosition();
			vector3 pos2 = objects[1]->getComponent<transform>()->getGlobalPosition();
			std::shared_ptr<graphPath> path = wgpf->getPath(pos1, pos2);
			std::list<vector3> resultingPath = path->points;

			componentHandle<debugLines> pathLines = debugLines::getLinesByTag(objects[0], PATH_LINE_TAG);
			std::vector<lineSegment> & lineBuffer = pathLines->getLineBuffer();
			lineBuffer.clear();

			if (path->isBad())
				return;

			vector3 color(1, 0, 1);
			float weight(.1f);
			
			lineBuffer.push_back(lineSegment(objects[0]->getComponent<transform>()->getGlobalPosition(), resultingPath.front(), color, weight));
			std::list<vector3>::iterator it = resultingPath.begin();
			std::list<vector3>::iterator previous = it;
			while (++it != resultingPath.end())
			{
				lineBuffer.push_back(lineSegment(*it, *previous, color, weight));
				++previous;
			}
		}
	}
}

std::pair<componentHandle<waypointNode>, componentHandle<waypointNode>> dynamicWaypointGraph::handlePathRequest(vector3 start, vector3 end)
{
	//Create a node at start and end points
	std::shared_ptr graphSpace = getSystemManager()->getSystem<sceneSystem>()->getCurrentScene().lock()->getSpace("WaypointGraph");
	if (graphSpace == nullptr)
		graphSpace = getSystemManager()->getSystem<sceneSystem>()->getCurrentScene().lock()->createSpace("WaypointGraph");

	std::shared_ptr<gameObject> startNode = makeNodeGameobject(&(*graphSpace), vector3(1, 1, 1), "solidblue.mtl");
	std::shared_ptr<gameObject> endNode = startNode->cloneObject();
	startNode->setName("StartNode");
	endNode->setName("EndNode");

	startNode->getComponent<transform>()->setPosition(start);
	endNode->getComponent<transform>()->setPosition(end);

	componentHandle<waypointNode> startWN = startNode->getComponent<waypointNode>();
	componentHandle<waypointNode> endWN = endNode->getComponent<waypointNode>();
	startWN->debugName = "StartPath";
	endWN->debugName = "EndPath";

	addPath(startWN, endWN);

	return std::make_pair(startWN, endWN);
}

void dynamicWaypointGraph::addPath(componentHandle<waypointNode> start, componentHandle<waypointNode> end)
{
	nodeConnection newConnection(start, end);

	std::list<intersection> intersections = findIntersections(newConnection);

	vector3 startNodePos = getNodePosition(start);

	//Base case - no intersections
	if (intersections.size() == 0)
	{
		connectNodes(start, end, false);
		validConnections.push_back(newConnection);
		addDebugLine(newConnection);
		return;
	}

	//connect start node to it's closest intersection
	nodeConnection closestCon = getClosestIntersection(startNodePos, intersections);

	//addPath(start, closestCon.firstNode);
	//addPath(start, closestCon.secondNode);

	//go left/right around intersecting object, node by node
	//keep list of looked at to avoid double checking or reversing
	std::list<componentHandle<waypointNode>> myObjectNodes = getObjectParameterNodes(closestCon.firstNode);

	std::queue<componentHandle<waypointNode>> toCheck;
	std::list<componentHandle<waypointNode>> lookedAt;
	lookedAt.push_back(closestCon.firstNode);
	lookedAt.push_back(closestCon.secondNode);
	toCheck.push(closestCon.firstNode);
	toCheck.push(closestCon.secondNode);

	//We're looking for nodes on the parameter that we've not looked at yet
	while (toCheck.empty() == false)
	{
		componentHandle<waypointNode> currentNode = toCheck.front();
		toCheck.pop();

		//Attempt to connect current to start
		//If intersect with self, skip
		nodeConnection startToCurrent(start, currentNode);

		intersections = findIntersections(startToCurrent);

		if (intersections.size() == 0)
		{
			connectNodes(start, currentNode, false);
			validConnections.push_back(startToCurrent);
			addDebugLine(startToCurrent);
		}
		else
		{
			closestCon = getClosestIntersection(getNodePosition(start), intersections);

			//If closest Connection node exists in parameter list, it shares an object parameter
			bool intersectsSelf = doesIntersectSelf(myObjectNodes, closestCon);

			//Not intersecting self means you must recurse start-current
			//as well as current-end
			if (!intersectsSelf)
			{
				addPath(start, currentNode);
			}
		}

		//Attempt to connect current to end
		nodeConnection currentToEnd(currentNode, end);

		intersections = findIntersections(currentToEnd);

		//No intersections, connect and recursive connect to end
		if (intersections.size() == 0)
		{
			addPath(currentNode, end);
			continue;
		}

		closestCon = getClosestIntersection(getNodePosition(currentNode), intersections);

		//If closest Connection node exists in parameter list, it shares an object parameter
		bool intersectsSelf = doesIntersectSelf(myObjectNodes, closestCon);

		//Not intersecting self means you must recurse start-current
		//as well as current-end
		if (!intersectsSelf)
		{
			addPath(start, currentNode);
			addPath(currentNode, end);
			continue;
		}

		//If closest connection is self, add neighbor to toCheck
		//-Need to continue searching around this object

		//Find next neighbor in line to attempt connection
		std::vector<connectedNode>::iterator it = currentNode->connections.begin();

		while (it != currentNode->connections.end())
		{
			if (it->sharesObjectParameter == true)
			{
				//Look for it in lookedAt
				std::list<componentHandle<waypointNode>>::iterator j = lookedAt.begin();
				while (j != lookedAt.end())
				{
					if (*j == it->node)
						break;
					++j;
				}
				//Not found in looked at, add to toCheck
				if (j == lookedAt.end())
				{
					toCheck.push(it->node);
					lookedAt.push_back(it->node);
				}
			}
			++it;
		}
	}
}

bool dynamicWaypointGraph::doesIntersectSelf(std::list<componentHandle<waypointNode>> myObjectNodes, nodeConnection closestConnection)
{
	for (std::list<componentHandle<waypointNode>>::iterator it = myObjectNodes.begin(); it != myObjectNodes.end(); ++it)
	{
		if (*it == closestConnection.firstNode)
		{
			return true;
		}
	}

	return false;
}

std::list<componentHandle<waypointNode>> dynamicWaypointGraph::getObjectParameterNodes(componentHandle<waypointNode> node)
{
	std::list<componentHandle<waypointNode>> toReturn;

	std::queue<componentHandle<waypointNode>> toCheck;
	std::list<componentHandle<waypointNode>> seen;

	toCheck.push(node);
	seen.push_back(node);

	//Build list based on neighbors that share an object parameter

	while (toCheck.empty() == false)
	{
		componentHandle<waypointNode> current = toCheck.front();
		std::string debugName = current->debugName;
		toCheck.pop();
		toReturn.push_back(current);

		std::vector<connectedNode> & connections = current->connections;

		for (std::vector<connectedNode>::iterator it = connections.begin(); it != connections.end(); ++it)
		{
			//If doesn't share parameter, skip it
			if (it->sharesObjectParameter == false)
				continue;

			//If seen already, skip it
			std::list<componentHandle<waypointNode>>::iterator j = seen.begin();
			while (j != seen.end())
			{
				if (it->node == *j)
					break;

				++j;
			}

			if (j != seen.end())
				continue;

			std::string newName = it->node->debugName;

			//new connected neighbor found
			toCheck.push(it->node);
			seen.push_back(it->node);
		}
	}

	return toReturn;
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
				nodeConnection newConnectionOne = connectNodesWithConnection(openConnection.secondNode, objectConnection.firstNode, false, true);
				nodeConnection newConnectionTwo = connectNodesWithConnection(openConnection.secondNode, objectConnection.secondNode, false, true);

				//Validate new paths
				validateConnection(newConnectionOne);
				validateConnection(newConnectionTwo);
			}
			//If >= 0, possible connection with first -> second from open path
			else
			{
				//Connect first open node to new object nodes 
				nodeConnection newConnectionOne = connectNodesWithConnection(openConnection.firstNode, objectConnection.firstNode, false, true);
				nodeConnection newConnectionTwo = connectNodesWithConnection(openConnection.firstNode, objectConnection.secondNode, false, true);

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

	//clear debug lines for each deleted node
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
				removeNodeFromNodesConnections(n1, n2);
			}
			else //match1
			{
				detachedNodes.push_back(n2);
				removeNodeFromNodesConnections(n2, n1);
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

	//Revalidate all connections that were invalidated by this node set
	for (invalidCheck = revalidate.begin(); invalidCheck != revalidate.end(); ++invalidCheck)
	{
		connectNodes(invalidCheck->firstNode, invalidCheck->secondNode, invalidCheck->onObjectParameter);
		validConnections.push_back(*invalidCheck);
		validateConnection(*invalidCheck);
	}
}

//======== Helper Functions ============================================================//
void dynamicWaypointGraph::createNodeSet(componentHandle<waypointNodeSet> set)
{
	//Make new node
	std::shared_ptr<gameObject> setObject = set->getGameObject().lock();
	std::shared_ptr<gameObject> node1 = makeNodeGameobject(setObject->getSpace(), vector3(.25f, .25f, .25f));


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

std::shared_ptr<gameObject> dynamicWaypointGraph::makeNodeGameobject(space * s, vector3 scale, std::string materialName) const
{
	std::shared_ptr<gameObject> toReturn = s->instantiateGameObject("Waypoint Graph Node");
	componentHandle<renderer> l_renderer_comp = toReturn->addComponent<renderer>();
	l_renderer_comp->setPrimitiveType(CUBE_FILE);
	l_renderer_comp->setProgramType(programType::enm_forward);
	toReturn->getComponent<renderer>()->setMaterialName(materialName);
	toReturn->getComponent<transform>()->setScale(scale);
	toReturn->addComponent<waypointNode>();

	return toReturn;
}

bool dynamicWaypointGraph::nodesAreConnected(componentHandle<waypointNode> n1, componentHandle<waypointNode> n2)
{
	for (int i = 0; i < n1->connections.size(); ++i)
	{
		if (n1->connections[i].node == n2)
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

	nodeConnection newConnection = connectNodesWithConnection(n1, n2, false, true);
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

nodeConnection dynamicWaypointGraph::connectNodesWithConnection(componentHandle<waypointNode> a, componentHandle<waypointNode> b, bool isObjectPath, bool isValid)
{
	nodeConnection newConnection = nodeConnection(a, b, isObjectPath, isValid);
	connectNodes(a, b, isObjectPath);
	return newConnection;
}

void dynamicWaypointGraph::connectNodes(componentHandle<waypointNode> a, componentHandle<waypointNode> b, bool sharesParameter)
{
	std::string namea = a->debugName;
	std::string nameb = b->debugName;
	if (nodesAreConnected(a, b))
		return;

	vector3 diffVector = getNodePosition(a) - getNodePosition(b);
	float cost = diffVector.distance();

	a->connections.push_back(connectedNode(b, cost, sharesParameter));
	b->connections.push_back(connectedNode(a, cost, sharesParameter));
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

	std::vector<connectedNode>::iterator conWalker;
	//clear connection from first node
	for (conWalker = nc.firstNode->connections.begin(); conWalker != nc.firstNode->connections.end(); ++conWalker)
	{
		if (conWalker->node == nc.secondNode)
		{
			nc.firstNode->connections.erase(conWalker);
			break;
		}
	}
	//clear connection from second node
	for (conWalker = nc.secondNode->connections.begin(); conWalker != nc.secondNode->connections.end(); ++conWalker)
	{
		if (conWalker->node == nc.firstNode)
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

		nodeConnection newConnection = connectNodesWithConnection(*it, *it2, true, true);
		validConnections.push_back(newConnection);

	    addDebugLine(newConnection, vector3(1, 0, 0));
	}
}

void dynamicWaypointGraph::deleteConnection(const nodeConnection & c)
{
	validConnections.remove(c);
	invalidConnections.remove(c);

	//Tell the nodes they are no longer connected
	removeNodeFromNodesConnections(c.firstNode, c.secondNode);
	removeNodeFromNodesConnections(c.secondNode, c.firstNode);
}

void dynamicWaypointGraph::removeNodeFromNodesConnections(componentHandle<waypointNode> toRemoveFrom, componentHandle<waypointNode> toRemove) const
{
	std::vector<connectedNode>::iterator it;
	for (it = toRemoveFrom->connections.begin(); it != toRemoveFrom->connections.end(); ++it)
	{
		if (it->node == toRemove)
		{
			toRemoveFrom->connections.erase(it);
			break;
		}
	}
}

nodeConnection dynamicWaypointGraph::getClosestIntersection(vector3 position, std::list<intersection> intersections)
{
	float closest = std::numeric_limits<float>::max();
	nodeConnection closestCon = intersections.front().oldConnection;

	for (std::list<intersection>::iterator it = intersections.begin(); it != intersections.end(); ++it)
	{
		float newDist = (it->intersectPoint - position).distanceSquared();
		if (newDist < closest)
		{
			closest = newDist;
			closestCon = it->oldConnection;
		}
	}

	return closestCon;
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
