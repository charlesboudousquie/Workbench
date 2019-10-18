#include "Repeater.hpp"
#ifndef TESTING_NODES

void Repeater::handleResult(BehaviorResult)
{
    // after running child once we will be able to exit
    this->phase = BehaviorPhase::DONE;
}

typeRT Repeater::onRender()
{
    return Decorator::decoratorOnRender();
}

void Repeater::Init()
{
    // set phase to ready for child
    Decorator::Init();

    // prepare to execute behavior
    this->phase = BehaviorPhase::PROGRESSING;
}

void Repeater::Update(float dt)
{
    // run child once
    child->tick(dt);
}
#endif