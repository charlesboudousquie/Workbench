/*!***************************************************************************************
\file       CameraSystem.hpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once
#include "../SystemBase.hpp"

class cameraBase;
class matrix4x4;

enum class cameraState
{
	enm_editor = 0,
	enm_game   = 1,
};

class cameraSystem : public systemBase
{

public:
	cameraSystem() = default;
	componentHandle<cameraBase> getMainCamera();

	static const std::string& getName() { static const std::string n("cameraSystem"); return n; }
	const std::string& name() const override { return getName(); }

	void enableEditorCamera(componentHandle<cameraBase> p_camera);
	void disableEditorCamera();

protected:
	/*!***************************************************************************************
	\brief  Initialize this system to hardly ever run.  It has no frame-based functionality.
	*****************************************************************************************/
	void onInitialize() override { setDesiredFrameTime(30.0f); }
private:

	  cameraState m_cameraState = cameraState::enm_game;
	  componentHandle<cameraBase> m_editorCamera;
	  

};