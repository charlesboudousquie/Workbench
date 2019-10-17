/*!***************************************************************************************
\file       BehaviorTree.hpp
\author     Charles Boudousquie
\date       9/24/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the object contains Behavior Nodes.
*****************************************************************************************/
#pragma once
#include <memory>

#include "AgentEncapsulator.hpp"

class Behavior;
class Agent;

typedef std::shared_ptr<Behavior> BehaviorPtr;

class BehaviorTree
{
    // current node aka where we are in the tree
    int currentNodeID;

    // map of Behavior ids to the actual behaviors
    std::map<int, BehaviorPtr> nodes;

    // main node of the tree
    std::shared_ptr<Behavior> root;

    // instead of having to update the agent pointer for each behavior 
    // in the tree, we just update this proxy instead
    AgentEncapsulator agentProxy;

    // the agents currently using this tree
    std::vector<std::shared_ptr<Agent>> agents;

    // name of tree
    std::string name;

    void RecursivelySetProxy(AgentEncapsulator* proxy, std::shared_ptr<Behavior> node);

    void ResetTree();
    void SetRoot(std::shared_ptr<Behavior>);

public:

    // return id of where we last left off in the node
    BehaviorPtr GetActiveNode();

    BehaviorTree();

    void SetName(const std::string& newName) { name = newName; }
    std::string GetName() { return name; }
    
    // give the tree its root node
    void SetUpTree(std::shared_ptr<Behavior>, std::map<int, BehaviorPtr>);

    // update tree
    void Update(float dt);

    // gives info to tree about its current node
    void SetCurrentNode(int);

    void AddAgent(std::shared_ptr<Agent> agent);
    std::vector<std::shared_ptr<Agent>> GetAgents();

};

