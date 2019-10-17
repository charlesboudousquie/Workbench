/*!***************************************************************************************
\file       RenderTargetScript.cpp
\author     Cody Cannell
\date       7/3/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the InputTestScript class member functions.
*****************************************************************************************/

//========Self Include==================================================================//
#include "RenderTargetScript.hpp"
#include "CameraScript.hpp"
//========1st Party Includes============================================================//
#include "../Components/InputComponent.hpp"
#include "../SceneManagement/Transform.hpp"
#include "../SceneManagement/GameObject.hpp"

#include "../Components/Camera.hpp"
#include "../Components/RenderTarget.hpp"
//#include "../Replay/JumbotronComponent.hpp"
//========3rd Party Includes============================================================//
//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//

/////////========================================================================/////////
///////// Namespace                                                              /////////
/////////========================================================================/////////

////////==========================================================================////////
////////  Struct / Class                                                          ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: classExample
\brief   This is a stub class to be used as a template / example for class formating.
*****************************************************************************************/

///////============================================================================///////
///////   Public/Protected/Private                                                 ///////
///////============================================================================///////

//////==============================================================================//////
//////    (Non-)Static                                                              //////
//////==============================================================================//////

/////================================================================================/////
/////     Functions / Data                                                           /////
/////================================================================================/////

renderTargetScript::renderTargetScript(typeRT p_data): scriptCPP(p_data)
{
}

void renderTargetScript::onUpdate()
{
    // get the input component
    componentHandle<inputComponent> l_input = getGameObject().lock()->getComponent<inputComponent>();
   // transform * l_tform = getGameObject().lock()->getComponent<transform>();
    componentHandle<renderTarget> l_comp = getGameObject().lock()->getComponent<renderTarget>();

    //float movementScale = 30.0f;
    //float rotationScale = 5.0f;

    //// check states and update transform
    //if (l_input->getButtonState("backward"))
    //{
    //    l_tform->offsetPosZ(movementScale * getDt());
    //}
    //if (l_input->getButtonState("forward"))
    //{
    //    l_tform->offsetPosZ(-movementScale * getDt());
    //}
    //if (l_input->getButtonState("left"))
    //{
    //    l_tform->offsetPosX(-movementScale * getDt());
    //}
    //if (l_input->getButtonState("right"))
    //{
    //    l_tform->offsetPosX(movementScale * getDt());
    //}
    //if (l_input->getButtonState("up"))
    //{
    //    l_tform->offsetPosY(movementScale * getDt());
    //}
    //if (l_input->getButtonState("down"))
    //{
    //    l_tform->offsetPosY(-movementScale * getDt());
    //}

    //if (l_input->getButtonState("scaleZUp"))
    //{
    //    l_tform->offsetScaleZ(movementScale * getDt());
    //}
    //if (l_input->getButtonState("scaleZDown"))
    //{
    //    l_tform->offsetScaleZ(-movementScale * getDt());
    //}
    //if (l_input->getButtonState("scaleXUp"))
    //{
    //    l_tform->offsetScaleX(movementScale * getDt());
    //}
    //if (l_input->getButtonState("scaleXDown"))
    //{
    //    l_tform->offsetScaleX(-movementScale * getDt());
    //}
    //if (l_input->getButtonState("scaleYUp"))
    //{
    //    l_tform->offsetScaleY(movementScale * getDt());
    //}
    //if (l_input->getButtonState("scaleYDown"))
    //{
    //    l_tform->offsetScaleY(-movementScale * getDt());
    //}

    if (l_input->getButtonState("play"))
    {
        l_comp->changePlayState(play_state::enum_playback);
    }
    else
    {
        //l_comp->changePlayState(play_state::enum_feed);
    }

}

