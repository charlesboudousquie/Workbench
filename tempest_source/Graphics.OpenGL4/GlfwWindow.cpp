/*!***************************************************************************************
\file       GlfwWindow.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#include "GlfwWindow.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GlfwWindowData.hpp"


glfwWindow::glfwWindow(int p_version_major, int p_version_minor, int p_width, int p_height, std::string p_title)
	: m_data{new glfwWindowData()}
{
	// initialize glfw library
	if (!glfwInit())
	{
		return;
	}

	// create a windowed mode window and the OpenGL context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, p_version_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, p_version_minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	m_data->m_window = glfwCreateWindow(p_width, p_height, p_title.c_str(), NULL, NULL);
	if (!m_data->m_window)
	{
		glfwTerminate();
		return;
	}

	// make the context current
	glfwMakeContextCurrent(m_data->m_window);

	// initialize glew
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		return;
	}

}

glfwWindow::~glfwWindow()
{
	glfwDestroyWindow(m_data->m_window);
	glfwTerminate();
	delete m_data;
}

std::pair<int, int> glfwWindow::getWindowSize() const
{
	std::pair<int, int> l_ret;
	glfwGetWindowSize(m_data->m_window, &l_ret.first, &l_ret.second);
	return l_ret;
}
