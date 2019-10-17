/*!***************************************************************************************
\file       Selector.hpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node returns true if one child succeeds, False if none succeed.
*****************************************************************************************/
#pragma once
#include "Composite.hpp"

class Selector : public Composite
{
public:
    void handleResult(BehaviorResult childResult) override;

    static typeRT onRender();

protected:

    void Init() override;
    void Update(float) override;
};