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
#include <vector>
#include <memory>

class gameObject;
typedef std::shared_ptr<gameObject> GameObjectPtr;

class behaviorTreeManipulatorInterface
{

public:
    //virtual void AddAgentToTree(const std::string& treeName, GameObjectPtr agent/*componentHandle<Agent> agent*/) = 0;
    virtual int getActiveNodeID(/*componentHandle<Agent>*/GameObjectPtr agent) const = 0;
    virtual std::vector<std::shared_ptr<gameObject>> getObjectsWithAgents() = 0;
};
