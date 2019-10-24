/*!***************************************************************************************
\file       JumpUp.cpp
\author     Charles Boudousquie
\date       10/18/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node makes the character jump upwards.
*****************************************************************************************/
#include "JumpUp.hpp"
#include "RigidBody.hpp"

typeRT JumpUp::onRender()
{
    return leafOnRender();
}

void JumpUp::Update(float)
{
    auto actor = GetTask()->GetActor();
    auto rigid = actor->getComponent<rigidBody>();

    // if no rigidbody then something went horribly wrong
    if (rigid.isNullptr())
    {
        std::string message = "Error object: " + actor->getName() + " HAS NO rigidbody";
        throw std::exception(message.c_str());
    }

    rigid->setVelocity({ 0,10,0 });

    GetTask()->SetResult(BehaviorResult::SUCCESS);
    GiveToParent(GetTask());
}
