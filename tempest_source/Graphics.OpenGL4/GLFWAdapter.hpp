/*!***************************************************************************************
\file       GLFWAdapter.hpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once
#include "DLLControl.hpp"
#include <windows.h>
#include <ExternalWindowInterface.hpp>
#include <RenderContextInterface.hpp>
#include <InputSourceInterface.hpp>

class glfwWindow;

class glfwAdapter : public externalWindowInterface, public renderContextInterface,
	public inputSourceInterface
{
public:
	/*GRAPHICSOPENGL4_API*/ glfwAdapter(glfwWindow * p_window);
	virtual ~glfwAdapter();

	// externalWindowInterface implementation
	void handleProcessEvents() override;
	void handleResizeRequest(int p_width, int p_height) override;
	void handleQuitRequest() override;

	// renderContextInterface implementation
	void handleSwapBuffersRequest() override;
	bool queryContextChanged() override;
	renderContextParameters queryContextParameters() override;
	void updateAssetPath(const std::string& newPath) override;
	void draw(const renderable& r) override;
	void updateCameraMatrices(const cameraData& c) override;

	// inputSourceInterface implementation
	void setKeyboardCallback(keyboardCallbackFunction p_function_ptr) override;
	void setMousePosCallback(mousePosCallbackFunction p_function_ptr) override;
	void setMouseButtonCallback(mouseButtonCallbackFunction p_function_ptr) override;
	void setGamepadButtonCallback(gamepadButtonCallbackFunction p_function_ptr) override;
	void setGamepadAxisCallbackFunction(gamepadAxisCallbackFunction p_function_ptr) override;
	scancodeWrapper mapLetterToScancode(unsigned p_letter) override;
	scancodeWrapper mapMouseButtonToScancode(unsigned p_button_id) override;

	/*GRAPHICSOPENGL4_API*/ bool isClosing() const;

private:
	void onGLFWKeyCallback(int p_key, int p_scancode, int p_action, int p_mods);
	void onGLFWMousePosCallback(double p_xpos, double p_ypos);
	void onGLFWMouseButtonCallback(int p_key, int p_action, int p_mods);


	glfwWindow * m_window;
	bool m_should_close = false;
	bool m_parameters_changed;
	renderContextParameters m_parameters;

	keyboardCallbackFunction m_keyboard_callback;
	mousePosCallbackFunction m_mouse_pos_callback;
	mouseButtonCallbackFunction m_mouse_button_callback;

	actionType * m_glfw_action_map;
};

