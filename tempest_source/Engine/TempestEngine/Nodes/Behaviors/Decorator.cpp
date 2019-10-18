/*!***************************************************************************************
\file       Decorator.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node is the base class to represent decorative nodes with 1 child.
*****************************************************************************************/
#include "Decorator.hpp"

std::vector<BehaviorPtr> Decorator::GetChildren()
{
    return { child };
}

BehaviorType Decorator::GetType()
{
    return BehaviorType::DECORATOR;
}

BehaviorPtr Decorator::getCurrentChild()
{
    return child;
}

typeRT Decorator::decoratorOnRender()
{
    typeRT l_data;

    std::vector<typeRT> l_non_render_members;

    typeRT l_non_render_data;
    l_non_render_data.setTypeName("Node Non Render Data");
    l_non_render_data.setVariableName("Node Non Render Data");

    typeRT l_inputs("Inputs", int(1));
    l_non_render_members.push_back(l_inputs);

    typeRT l_outputs("Outputs", int(1));
    l_non_render_members.push_back(l_outputs);

    //Push back non render data
    for (auto i_member : l_non_render_members)
    {
        l_non_render_data.insertMember(i_member);
    }

    l_data.insertMember(l_non_render_data);

    return l_data;
}

void Decorator::addChild(BehaviorPtr newChild)
{
    if (child == nullptr)
    {
        child = newChild;
        //child->setParent(this);
    }
    else
    {
        throw std::exception("Tried to add more than 1 child to a decorator node.");
    }
}

void Decorator::Init()
{
    assert(child != nullptr);

    // start up node
    auto task = GetTask();
    BehaviorState state(this->getId(), 0, BehaviorPhase::STARTING);
    task->Push_State(state);
}




