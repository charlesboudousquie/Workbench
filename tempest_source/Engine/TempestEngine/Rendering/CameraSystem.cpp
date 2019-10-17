/*!***************************************************************************************
\file       CameraSystem.cpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "../Components/Camera.hpp"
#include "../GameObjectFiltering/GameObjectFilter.hpp"
#include "../SceneManagement/GameObject.hpp"
#include "../SystemManager.hpp"
#include "CameraBase.hpp"
#include "CameraSystem.hpp"



componentHandle<cameraBase> cameraSystem::getMainCamera()
{
	if (m_cameraState == cameraState::enm_editor) // return editor camera
	{
		return m_editorCamera;
	}
	else // get main camera from object system;
	{
		GameObjectFiltering::componentTypeSet l_pattern;
		l_pattern.setType(camera::getType());
		auto l_listCameras = getSystemManager()->getGameObjectGatherer()->getList(l_pattern);

		for (auto & l_cameraObj : l_listCameras)
		{
			if (l_cameraObj->getName() == "Main Camera")
			{
				return componentHandle<cameraBase>::castHandle(l_cameraObj->getComponent<camera>());
			}
		}

	}

	return componentHandle<cameraBase>();
}

void cameraSystem::enableEditorCamera(componentHandle<cameraBase> p_camera)
{
	m_editorCamera = p_camera;
	m_cameraState = cameraState::enm_editor;
}

void cameraSystem::disableEditorCamera()
{
	m_cameraState = cameraState::enm_game;
	m_editorCamera = componentHandle<cameraBase>();
}





