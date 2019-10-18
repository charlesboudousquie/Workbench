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

class Agent;
class BehaviorTree;
class BehaviorTreeBuilder;
class BehaviorTreeDataBase;

class BehaviorTreeManager final : public systemBase
{
    typedef std::shared_ptr<Agent> AgentPtr;
    typedef std::shared_ptr<BehaviorTree> BehaviorTreePtr;
    typedef std::shared_ptr<BehaviorTreeBuilder> BehaviorTreeBuilderPtr;
    typedef std::shared_ptr<BehaviorTreeDataBase> BehaviorTreeDataBasePtr;

    // there should only be 1 tree builder
    BehaviorTreeBuilderPtr builder;
    BehaviorTreeDataBasePtr dataBase;

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

    // activate all trees within manager
    void Activate();
    
    void Deactivate()
    {
        shouldUpdate = false;
    }
    /*!***************************************************************************************
    \brief  An Agent component is assigned 1 tree, so they can be linked directly together
    *****************************************************************************************/
    void linkAgentComponentToTree(std::shared_ptr<Agent> agent, const std::string& treeName);

    // returns id of node that an agent is working on
    int getCurrentNodeID(AgentPtr);

};


