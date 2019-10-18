/*!***************************************************************************************
\file       BehaviorTree.hpp
\author     Charles Boudousquie
\date       9/24/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the object contains Behavior Nodes.
*****************************************************************************************/
#include "BehaviorTree.hpp"
#include "BehaviorTaskProxy.hpp"
#include "Nodes/Behaviors/Behavior.hpp"
#include "AgentEncapsulator.hpp"
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
    //this->task = std::make_shared<BehaviorTaskProxy>();

    // set root
    this->root = root_;

    // proxy is already set in constructor
    //RecursivelySetProxy(this->taskProxy, this->root);

}

//void BehaviorTree::RecursivelySetProxy(BehaviorTaskProxyPtr proxy, BehaviorPtr node)
//{
//    // set agent proxy
//    node->setTaskProxy(proxy);
//
//    // get child nodes
//    auto children = node->GetChildren();
//
//    // if any children then we recursively set proxy for them
//    for (auto child : children)
//    {
//        RecursivelySetProxy(proxy, child);
//    }
//}

//BehaviorTaskProxyPtr BehaviorTree::GetProxyTask()
//{
//    return this->taskProxy;
//}

BehaviorPtr BehaviorTree::GetRoot()
{
    return root;
}

BehaviorTree::BehaviorTree()
{
    //this->taskProxy = std::make_shared<BehaviorTaskProxy>();
}

void BehaviorTree::SetName(const std::string & newName)
{
    name = newName;
}

std::string BehaviorTree::GetName()
{
    return this->name;
}

void BehaviorTree::SetRoot(BehaviorPtr root_)
{
    root = root_;
}
