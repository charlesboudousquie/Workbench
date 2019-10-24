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
#include <memory>

class BehaviorTree;
class BehaviorTreeBuilder;
//class BehaviorTreeDataBase;

class gameObject;
typedef std::shared_ptr<gameObject> GameObjectPtr;

class BehaviorTreeManager final : public systemBase
{
    typedef std::shared_ptr<BehaviorTree> BehaviorTreePtr;
    typedef std::shared_ptr<BehaviorTreeBuilder> BehaviorTreeBuilderPtr;
    //typedef std::shared_ptr<BehaviorTreeDataBase> BehaviorTreeDataBasePtr;

    // there should only be 1 tree builder
    BehaviorTreeBuilderPtr builder;
    //BehaviorTreeDataBasePtr dataBase;

    std::unordered_map<std::string, BehaviorTreePtr> trees;

    // add any new actors that were recently created
    // and remove any actors that have been removed from the scene
    //void UpdateActors();

    // gets tree if it exists otherwise returns nullptr
    BehaviorTreePtr GetTree(const std::string&);

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
    
    // returns all game objects that have an agent component
    std::vector<std::shared_ptr<gameObject>> getAgentGameObjects();

    // returns id of node that an agent is working on
    int getCurrentNodeID(GameObjectPtr);

    // remove actor from manager
    //void RemoveActor(GameObjectPtr);
};


