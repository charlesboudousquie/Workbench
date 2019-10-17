/*!***************************************************************************************
\file       BehaviorTreeManager.hpp
\author     Charles Boudousquie
\date       9/24/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the Manager that controls any behavior tree an agent may use.
*****************************************************************************************/
#pragma once
#include <SystemBase.hpp>

//#include "Reflection.hpp"

class Agent;
class BehaviorTree;
class BehaviorTreeBuilder;

class BehaviorTreeManager final : public systemBase
{
    typedef std::shared_ptr<BehaviorTree> BehaviorTreePtr;

    // all trees in use, should be unique
    std::map<std::string, BehaviorTreePtr> trees;

    //BehaviorTreePtr tree;

    // there should only be 1 tree builder
    std::shared_ptr<BehaviorTreeBuilder> builder;

    // list of all agents in existence
    //std::vector<std::shared_ptr<Agent>> agents;

    bool shouldUpdate;

public:

    /*!*******************************************************************************
    \brief  Default constructor
    *********************************************************************************/
    BehaviorTreeManager();

    /*!*******************************************************************************
    \brief  Retrieves the type of component this instance is. SceneSystem requirement.
    \return componentType - The type of this component.
    *********************************************************************************/
    virtual const std::string& name() const override;

    /*!*******************************************************************************
    \brief  Returns name of this system.
    *********************************************************************************/
    static const std::string& getName() 
    { 
        static const std::string n("BehaviorTreeManager"); return n; 
    }

    /*!***************************************************************************************
    \brief  Updates all trees.
    *****************************************************************************************/
    void onUpdate() override;

    /*!***************************************************************************************
    \brief  Tells treebuilder to build trees from disk
    *****************************************************************************************/
    virtual void onInitialize()override;

    void ActivateTree(const std::string& treeName);
    

    void Deactivate()
    {
        shouldUpdate = false;
    }
    /*!***************************************************************************************
    \brief  An Agent component is assigned 1 tree, so they can be linked directly together
    *****************************************************************************************/
    void linkAgentComponentToTree(std::shared_ptr<Agent> agent, const std::string& treeName);

    // 1 tree for now, gets its currently running node
    int getCurrentNodeID();

};


