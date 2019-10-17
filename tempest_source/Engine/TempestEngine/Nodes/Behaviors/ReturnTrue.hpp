/*!***************************************************************************************
\file       ReturnTrue.hpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node returns true regardless of result of child.
*****************************************************************************************/
#pragma once
#include "Decorator.hpp"

class ReturnTrue : public Decorator
{
public:
    
    void handleResult(BehaviorResult) override;

    static typeRT onRender();

    void Update(float) override;
};