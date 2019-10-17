/*!***************************************************************************************
\file       BehaviorTreeManipulatorInterface.hpp
\author     Charles Boudousquie
\date       10/11/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Interface of the manipulator that talks to the BehaviorTreeManager
*****************************************************************************************/
#pragma once
#include <string>

class Agent;

class behaviorTreeManipulatorInterface
{

public:
    virtual void AddAgentToTree(const std::string& treeName, std::shared_ptr<Agent> agent) = 0;
    virtual int getActiveNodeID() const = 0;

};
