/*!***************************************************************************************
\file       Composite.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This class represents a node in a behavior tree that has multiple child nodes.
*****************************************************************************************/
#include "Composite.hpp"

typeRT Composite::compositeOnRender()
{
    // final typert data to return
    typeRT l_data;
    
    // typeRT data that is not displayed but does affect functionality of node
    std::vector<typeRT> l_non_render_members;

    typeRT l_non_render_data;
    l_non_render_data.setTypeName("Node Non Render Data");
    l_non_render_data.setVariableName("Node Non Render Data");

    typeRT l_inputs("Inputs", int(1));
    l_non_render_members.push_back(l_inputs);

    typeRT l_outputs("Outputs", int(6));  // ask ryan about implementing max child nodes
    l_non_render_members.push_back(l_outputs);

    //Push back non render data
    for (auto i_member : l_non_render_members)
    {
        l_non_render_data.insertMember(i_member);
    }

    l_data.insertMember(l_non_render_data);

    return l_data;
}

void Composite::SetPhaseOfChildren(BehaviorPhase phase_)
{
    for (auto child : childNodes)
    {
        child->SetPhase(phase_);
    }
}

void Composite::Init()
{
    // set Phase of ourself as ready
    Behavior::Init();

    // set Phase of children as ready to go
    SetPhaseOfChildren(BehaviorPhase::STARTING);

    // set current child as beginning point
    currentChild = childNodes.begin();
}

Composite::Composite()
{}

std::vector<BehaviorPtr> Composite::GetChildren()
{
    return childNodes;
}

BehaviorPtr Composite::getCurrentChild()
{
    return *currentChild;
}

void Composite::addChild(BehaviorPtr newChild)
{
    childNodes.push_back(newChild);

    //newChild->setParent(this);
}

void Composite::removeChild(BehaviorPtr child)
{
    std::remove(childNodes.begin(), childNodes.end(), child);
}

void Composite::clearChildren()
{
    childNodes = {}; // clears it
}
