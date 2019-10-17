/*!***************************************************************************************
\file       BehaviorAgent.hpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This class represents an agent that traverses a behavior tree.
*****************************************************************************************/
#pragma once

#include <map>
#include <any>


class BehaviorAgent
{
public:


private:
    // make sure /GF is in comiler switches, it enables string pooling
   // so "hello" == "hello"
    std::map<const char*, std::any> blackBoard;


};