/*!***************************************************************************************
\file       Repeater.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node restarts and processes its child node every time no matter what.
            Because of this it is highly suggested to use this as the root of the
            behavior tree.
*****************************************************************************************/
#include "Decorator.hpp"

class Repeater : public Decorator
{
public:

    void handleResult(BehaviorResult)override;

    static typeRT onRender();

    // reprocesses its child every time it returns a result
    void Init() override;
    void Update(float dt) override;

};
