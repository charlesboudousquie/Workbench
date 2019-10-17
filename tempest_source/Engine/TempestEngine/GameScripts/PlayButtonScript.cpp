/*!***************************************************************************************
\file       PlayButtonScript.cpp
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "PlayButtonScript.hpp"

playButtonScript::playButtonScript()
{
}

playButtonScript::playButtonScript(typeRT p_data)
{
	updateFromTypeRT(p_data);
}

typeRT playButtonScript::toTypeRT() const
{
	// Generate the base script data
	typeRT l_result = scriptCPP::toTypeRT();

	// add data to the l_scriptData object here.
	l_result.insertMember(typeRT("sceneName", m_scene_name));

	return l_result;
}

void playButtonScript::updateFromTypeRT(typeRT& p_type)
{
	auto l_members = p_type.members();

	if(l_members.find("sceneName") != l_members.end())
	{
		m_scene_name = p_type.member("sceneName").getString();
	}
}

void playButtonScript::onStart()
{
	m_button = getComponent<button>();
}

void playButtonScript::onUpdate()
{
	if(!m_button.isNullptr() && m_button->isPressed())
	{
    auto l_systemManager = getSystemManager();
    //If the play button is pressed, and we are already in the desired scene, then don't switch to it.
    if (l_systemManager->getSystem<sceneSystem>()->getCurrentScene().lock()->getName() != m_scene_name)
    {
      auto l_scene = l_systemManager->getSystem<sceneSystem>()->getScene(m_scene_name);
      if (l_scene == nullptr)
      {
        // load the scene
        l_scene = getSystemManager()->getSystem<sceneSystem>()->loadScene(m_scene_name);
      }
      getSystemManager()->getSystem<sceneSystem>()->setCurrentScene(l_scene);
    }
    //Hide the UI instead of switching scenes
    else
    {
      
    }
	}
}
