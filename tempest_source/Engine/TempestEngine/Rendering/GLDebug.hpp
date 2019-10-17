/*!***************************************************************************************
\file       GLDebug.hpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is a work in progress example sheet for section headers.
*****************************************************************************************/
#pragma once

#include "GL/glew.h"
#include "GL/gl.h"

#define ASSERT(condition) { if(!(condition)){ std::cerr << "ASSERT FAILED: " << #condition << " @ " << __FILE__ << " (" << __LINE__ << ")" << std::endl; } }

struct GLDebug
{
	static void InitializeDebugCallback();
    /*!***************************************************************************************
    \brief  Gets the last error of program
    \param  p_handle - Handle to the program object
    *****************************************************************************************/
    static void GetLastProgramError(GLint p_handle);

    /*!***************************************************************************************
    \brief  Gets the last error
    \param  p_handle - handle to the GPU shader object
    *****************************************************************************************/
    static void GetLastShaderError(GLint p_handle);

    static void GetLastProgramLinkError(GLint p_handle);

    static void GetLastProgramPipelineError(GLint p_handle);

    static void getLastError();

    static void getLastFrameBufferError();

	static void GLAPIENTRY message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param);
};


