/*!***************************************************************************************
\file       AgentEncapsultor.hpp
\author     Charles Boudousquie
\date       9/24/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This helper class keeps track of what agent it is pointing at. 
*****************************************************************************************/
#pragma once
#include <memory>
class Agent;


class AgentEncapsulator
{

    std::shared_ptr<Agent> agent;
public:
    void Set(std::shared_ptr<Agent>);
    std::shared_ptr<Agent> Get();
};

typedef std::shared_ptr<AgentEncapsulator> AgentEncapsulatorPtr;
