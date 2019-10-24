/*!***************************************************************************************
\file       BehaviorTreeManipulator.hpp
\author     Charles Boudousquie
\date       10/11/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Manipulator to talk to the BehaviorTreeManager
*****************************************************************************************/
#pragma once
#include "BehaviorTreeManipulatorInterface.hpp"

class systemManagerInterface;

class BehaviorTreeManipulator : public behaviorTreeManipulatorInterface
{
    systemManagerInterface* m_system_manager;
public:

    BehaviorTreeManipulator(systemManagerInterface*);

    //void AddAgentToTree(const std::string& treeName, /*componentHandle<Agent>*/GameObjectPtr agent) override;

    // get active node in BehaviorManager system
    int getActiveNodeID(/*componentHandle<Agent>*/GameObjectPtr agent) const override;

    // get all game objects that have an agent component
    std::vector<std::shared_ptr<gameObject>> getObjectsWithAgents();

};

