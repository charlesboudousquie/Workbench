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

class systemManager;

class BehaviorTreeManipulator : public behaviorTreeManipulatorInterface
{
    systemManager* m_system_manager;
public:

    BehaviorTreeManipulator(systemManager*);

    void AddAgentToTree(const std::string& treeName, std::shared_ptr<Agent> agent) override;

    //void testTree(const std::string & treeName) const override;

    // get active node in BehaviorManager system, and get the following from it
    // 1. result of node
    // 2. id of node
    int getActiveNodeID() const override;
};

