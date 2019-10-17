/*!***************************************************************************************
\file       Leaf.hpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This class represents a node with no children. It is also known as an action 
            node.
*****************************************************************************************/
#pragma once
#include "Behavior.hpp"


class Leaf : public Behavior
{
public:

    static typeRT leafOnRender();

    void handleResult(BehaviorResult childResult) override;

    // returns null since it has no active child
    BehaviorPtr getCurrentChild() override;

    // returns empty vector since leaves have no child nodes
    std::vector<BehaviorPtr> GetChildren() override;

    // no children should ever be added to leaf node
    void addChild(BehaviorPtr)override;
};
