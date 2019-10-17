/*!***************************************************************************************
\file       Timer.hpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node is a simple Decorator timer that activates its child node when
            timer hits 0.
*****************************************************************************************/
#pragma once
#include "Decorator.hpp"

class Timer : public Decorator
{

    float timer = 2.0f;
    
public:

    static typeRT onRender();

    void handleResult(BehaviorResult) override;

    void Update(float) override;
    
};

