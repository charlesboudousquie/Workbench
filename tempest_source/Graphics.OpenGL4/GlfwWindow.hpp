/*!***************************************************************************************
\file       GlfwWindow.hpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once

#include "DLLControl.hpp"
#include <string>

struct glfwWindowData;

class glfwWindow
{
public:
	/*GRAPHICSOPENGL4_API*/ glfwWindow(int p_version_major, int p_version_minor, int p_width, int p_height, std::string p_title);
	virtual ~glfwWindow();

	std::pair<int, int> getWindowSize() const;

private:
	glfwWindowData * m_data;

	friend class glfwAdapter;
};