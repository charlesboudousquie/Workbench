/*!***************************************************************************************
\file       BehaviorTree.hpp
\author     Charles Boudousquie
\date       9/24/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the object contains Behavior Nodes.
*****************************************************************************************/
#include "BehaviorTree.hpp"
#include "Nodes/Behaviors/Behavior.hpp"
#include "Agent.hpp"

BehaviorTaskPtr BehaviorTree::GetTask()
{
    return this->task;
}

void BehaviorTree::SetTask(BehaviorTaskPtr t)
{
    this->task = t;
}

void BehaviorTree::SetUpTree(BehaviorPtr root_, std::map<int, BehaviorPtr> nodes_)
{
    root = root_;
    RecursiveSetParentTree(root.get());
}

BehaviorPtr BehaviorTree::GetRoot()
{
    return root;
}

BehaviorTree::BehaviorTree(){}

void BehaviorTree::SetName(const std::string & newName)
{
    name = newName;
}

std::string BehaviorTree::GetName()
{
    return this->name;
}

void BehaviorTree::RecursiveSetParentTree(Behavior* behavior)
{
    behavior->setParentTree(this);

    auto children = behavior->GetChildren();

    for (auto child : children)
    {
        RecursiveSetParentTree(child.get());
    }
}
