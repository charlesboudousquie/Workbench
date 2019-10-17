/*!***************************************************************************************
\file       BehaviorTreeManipulator.cpp
\author     Charles Boudousquie
\date       10/11/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Manipulator to talk to the BehaviorTreeManager
*****************************************************************************************/
#include "BehaviorTreeManipulator.hpp"
#include "BehaviorTreeManager.hpp"
#include "SystemManager.hpp"

BehaviorTreeManipulator::BehaviorTreeManipulator(systemManager * manager_) : m_system_manager(manager_)
{}

void BehaviorTreeManipulator::AddAgentToTree(const std::string & treeName, std::shared_ptr<Agent> agent)
{
    // get bt manager
    auto btManager = m_system_manager->getSystem<BehaviorTreeManager>();
    btManager->linkAgentComponentToTree(agent, treeName);
}


void BehaviorTreeManipulator::testTree(const std::string & treeName) const
{
    auto BTManager = m_system_manager->getSystem<BehaviorTreeManager>();
    BTManager->ActivateTree(treeName);
}

int BehaviorTreeManipulator::getActiveNodeID() const
{
    // get behavior tree manager system
    auto BTManager = m_system_manager->getSystem<BehaviorTreeManager>();

    return BTManager->getCurrentNodeID();
}