/*!***************************************************************************************
\file       GLFWAdapter.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "GLFWAdapter.hpp"
#include "GlfwWindow.hpp"
#include <functional>
#include <cstring> // std::memset

#include "GlfwWindowData.hpp"

#include <glfw3.h>

static renderContextParameters getWindowRect(glfwWindow * p_window)
{
	renderContextParameters l_winDim;
	std::tie(l_winDim.width, l_winDim.height) = p_window->getWindowSize();
	return l_winDim;
}

glfwAdapter::glfwAdapter(glfwWindow * p_window)
	: m_window{p_window}
	, m_parameters_changed{true}
	, m_parameters{getWindowRect(p_window)}
	, m_keyboard_callback{ nullptr }
	, m_mouse_pos_callback{ nullptr }
	, m_glfw_action_map{new actionType[GLFW_REPEAT + 1]}
{
	glfwSetWindowUserPointer(m_window->m_data->m_window, this);

	glfwSetWindowCloseCallback(m_window->m_data->m_window,
		[](GLFWwindow * p_window)
	{
		static_cast<glfwAdapter *>(glfwGetWindowUserPointer(p_window))
			->m_should_close = true;
	});
	glfwSetWindowSizeCallback(m_window->m_data->m_window,
		[](GLFWwindow * p_window, int p_width, int p_height)
	{
		// update the parameters
		auto l_ptr = static_cast<glfwAdapter *>(glfwGetWindowUserPointer(p_window));
		l_ptr->m_parameters.width = p_width;
		l_ptr->m_parameters.height = p_height;
		l_ptr->m_parameters_changed = true;
	});
	glfwSetKeyCallback(m_window->m_data->m_window,
		[](GLFWwindow * p_window, int p_key, int p_scancode, int p_action, int p_mods)
	{
		static_cast<glfwAdapter *>(glfwGetWindowUserPointer(p_window))
			->onGLFWKeyCallback(p_key, p_scancode, p_action, p_mods);
	});
	glfwSetCursorPosCallback(m_window->m_data->m_window,
		[](GLFWwindow * p_window, double p_xpos, double p_ypos)
	{
		static_cast<glfwAdapter *>(glfwGetWindowUserPointer(p_window))
			->onGLFWMousePosCallback(p_xpos, p_ypos);
	});
	glfwSetMouseButtonCallback(m_window->m_data->m_window,
		[](GLFWwindow * p_window, int p_button, int p_action, int p_mods)
	{
		static_cast<glfwAdapter *>(glfwGetWindowUserPointer(p_window))
			->onGLFWMouseButtonCallback(p_button, p_action, p_mods);
	});

	// populate the action map
	m_glfw_action_map[GLFW_PRESS] = actionType::Pressed;
	m_glfw_action_map[GLFW_RELEASE] = actionType::Released;
	m_glfw_action_map[GLFW_REPEAT] = actionType::Repeat;

	
}

glfwAdapter::~glfwAdapter()
{
	delete [] m_glfw_action_map;
}

void glfwAdapter::handleProcessEvents()
{
	glfwPollEvents();
}

void glfwAdapter::handleResizeRequest(int p_width, int p_height)
{
	glfwSetWindowSize(m_window->m_data->m_window, p_width, p_height);
}

void glfwAdapter::handleQuitRequest()
{
	// if engine requests a close, we close
	m_should_close = true;
}

bool glfwAdapter::isClosing() const
{
	return m_should_close;
}

void glfwAdapter::onGLFWKeyCallback(int /*p_key*/, int p_scancode, int p_action, int p_mods)
{
	scancodeWrapper l_scancode = { 0 };
	l_scancode.scancode = p_scancode;
	m_keyboard_callback(l_scancode, m_glfw_action_map[p_action], p_mods);
}

void glfwAdapter::onGLFWMousePosCallback(double p_xpos, double p_ypos)
{
	m_mouse_pos_callback(p_xpos, p_ypos);
}

void glfwAdapter::onGLFWMouseButtonCallback(int p_key, int p_action, int p_mods)
{
	scancodeWrapper l_scancode = mapMouseButtonToScancode(p_key);
	m_keyboard_callback(l_scancode, m_glfw_action_map[p_action], p_mods);
}

void glfwAdapter::handleSwapBuffersRequest()
{
	glfwSwapBuffers(m_window->m_data->m_window);
}

bool glfwAdapter::queryContextChanged()
{
	return m_parameters_changed;
}

renderContextParameters glfwAdapter::queryContextParameters()
{
	m_parameters_changed = false;
	return m_parameters;
}

void glfwAdapter::updateAssetPath(const std::string& /*newPath*/)
{
}

void glfwAdapter::draw(const renderable & /*r*/)
{
}

void glfwAdapter::updateCameraMatrices(const cameraData & /*c*/)
{
}

void glfwAdapter::setKeyboardCallback(keyboardCallbackFunction p_function_ptr)
{
	m_keyboard_callback = p_function_ptr;
}

void glfwAdapter::setMousePosCallback(mousePosCallbackFunction p_function_ptr)
{
	m_mouse_pos_callback = p_function_ptr;
}

void glfwAdapter::setMouseButtonCallback(mouseButtonCallbackFunction p_function_ptr)
{
	m_mouse_button_callback = p_function_ptr;
}

void glfwAdapter::setGamepadButtonCallback(gamepadButtonCallbackFunction p_function_ptr)
{
	//do nothing: no gamepad through glfw
	(void)p_function_ptr;
}

void glfwAdapter::setGamepadAxisCallbackFunction(gamepadAxisCallbackFunction p_function_ptr)
{
	//do nothing: no gamepad through glfw
	(void)p_function_ptr;
}

scancodeWrapper glfwAdapter::mapLetterToScancode(unsigned p_letter)
{
	scancodeWrapper result = { 0 };
	// result.scancode = MapVirtualKey(p_letter, MAPVK_VK_TO_VSC);
	result.scancode = glfwGetKeyScancode(p_letter);
	return result;
}

scancodeWrapper glfwAdapter::mapMouseButtonToScancode(unsigned p_button_id)
{
	// mouse buttons don't actually have scancodes, so we'll just offset them
	// to be outside the range of the normal keyboards scancodes
	//   Windows Virtual Keys max at 0xFF
	scancodeWrapper result = { GLFW_KEY_LAST + 1 + p_button_id };
	return result;
}
