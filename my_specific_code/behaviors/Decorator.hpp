/*!***************************************************************************************
\file       Decorator.hpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node is the base class to represent decorative nodes with 1 child.
*****************************************************************************************/
#pragma once
#include "Behavior.hpp"

class Decorator : public Behavior
{
public:

    std::vector<BehaviorPtr> GetChildren() override;

protected:

    BehaviorType GetType() override;

    BehaviorPtr getCurrentChild() override;

    static typeRT decoratorOnRender();

    virtual void addChild(BehaviorPtr newChild) override;

    // automatically adds new state for child to work with
    // and sends task to child
    void Init() override;

    BehaviorPtr child = nullptr;
};

