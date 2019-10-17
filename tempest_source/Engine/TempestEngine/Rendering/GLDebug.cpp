/*!***************************************************************************************
\file       GLDebug.cpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is a work in progress example sheet for section headers.
*****************************************************************************************/
#include "../Precompiled.h"
#include "GLDebug.hpp"
#include <Logger.hpp>

/*!***************************************************************************************
\brief  Prints Error from defines
\param error - error number
\return string of the error
*****************************************************************************************/

std::string GLErrorToString(GLenum error)
{
    switch (error)
    {
    case GL_NO_ERROR:
        return "GL_NO_ERROR";
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";
    default:
        return "unknown error";
    }
}

std::string GLFramebufferErrorToString(GLenum error)
{
    switch (error)
    {
    case GL_FRAMEBUFFER_COMPLETE:
        return "GL_FRAMEBUFFER_COMPLETE";
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
    case GL_FRAMEBUFFER_UNSUPPORTED:
        return "GL_FRAMEBUFFER_UNSUPPORTED";
    default:
        return "unknown error";
    }
}





void GLDebug::GetLastShaderError(GLint p_handle)
{

    GLint l_success = 0;
    GLint l_legth = 0;



    glGetShaderiv(p_handle, GL_COMPILE_STATUS, &l_success);
    if (l_success == GL_FALSE)
    {
        logger("GLDebug").debug() << GLErrorToString(glGetError()) << std::endl;
        glGetShaderiv(p_handle, GL_INFO_LOG_LENGTH, &l_legth);

        GLchar * l_infoLog = new GLchar[l_legth];

        glGetShaderInfoLog(p_handle, l_legth, &l_legth, l_infoLog);
        logger("GLDebug").debug() << "ERROR::SHADER_" << p_handle << "::COMPILATION_FAILED\n" << l_infoLog << std::endl;

        delete[] l_infoLog;
    }
    else
    {
        logger("GLDebug").debug() << "SHADER_" << p_handle << ":no error\n" << std::endl;
    }
}


void GLDebug::InitializeDebugCallback()
{
	glEnable(GL_DEBUG_OUTPUT);
	GLuint unusedIds{ 0 };
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(&GLDebug::message_callback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
		&unusedIds, true);
}

void GLDebug::GetLastProgramError(GLint p_handle)
{
    GLint l_success = 0;
    GLint l_legth = 512;
    GLchar l_infoLog[512];

    glGetProgramiv(p_handle, GL_COMPILE_STATUS, &l_success);

    if (l_success == GL_FALSE)
    {
        logger("GLDebug").debug() << GLErrorToString(glGetError()) << std::endl;
        glGetProgramiv(p_handle, GL_INFO_LOG_LENGTH, &l_legth);

        //bool isShader = glIsShader(handle);
        glGetProgramInfoLog(p_handle, l_legth, &l_legth, l_infoLog);
        logger("GLDebug").debug() << "ERROR::PROGRAM::COMPILATION_FAILED\n" << l_infoLog << std::endl;
    }
}

void GLDebug::GetLastProgramLinkError(GLint p_handle)
{
    GLint l_success = 0;
    GLint l_legth = 512;
    GLchar l_infoLog[513]={'\0'};

    glGetProgramiv(p_handle, GL_LINK_STATUS, &l_success);

    if (l_success == GL_FALSE)
    {
        logger("GLDebug").debug() << GLErrorToString(glGetError()) << std::endl;
		GLint log_length;
        glGetProgramiv(p_handle, GL_INFO_LOG_LENGTH, &log_length);

		log_length = std::min(log_length, l_legth);

        //bool isShader = glIsShader(handle);
        glGetProgramInfoLog(p_handle, l_legth, &log_length, l_infoLog);
        logger("GLDebug").debug() << "ERROR::PROGRAM::LINKING_FAILED\n" << l_infoLog << std::endl;
    }
}

void GLDebug::GetLastProgramPipelineError(GLint p_handle)
{
    GLint l_success = 0;
    GLint l_legth = 512;
    GLchar l_infoLog[512];

    glGetProgramPipelineiv(p_handle, GL_COMPILE_STATUS, &l_success);

    if (l_success == GL_FALSE)
    {
        logger("GLDebug").debug() << GLErrorToString(glGetError()) << std::endl;
        glGetProgramPipelineiv(p_handle, GL_INFO_LOG_LENGTH, &l_legth);

        //bool isShader = glIsShader(handle);
        glGetProgramPipelineInfoLog(p_handle, l_legth, &l_legth, l_infoLog);
        logger("GLDebug").debug() << "ERROR::PROGRAM::COMPILATION_FAILED\n" << l_infoLog << std::endl;
    }
}



void GLDebug::getLastError()
{
    logger("GLDebug").debug() << GLErrorToString(glGetError()) << std::endl;
}


void GLDebug::getLastFrameBufferError()
{
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    logger("Framebuffer").debug() << GLFramebufferErrorToString(status);

}

 void GLAPIENTRY GLDebug::message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
	 if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
	UNREFERENCED_PARAMETER(length);
	UNREFERENCED_PARAMETER(user_param);
	auto const src_str = [source]() {
		switch (source)
		{
		case GL_DEBUG_SOURCE_API: return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
		case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
		case GL_DEBUG_SOURCE_OTHER: return "OTHER";
		default: return "No_Error";
		}
	}();

	auto const type_str = [type]() {
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR: return "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER: return "MARKER";
		case GL_DEBUG_TYPE_OTHER: return "OTHER";
		default: return "No_Error";
		}
	}();

	auto const severity_str = [severity]() {
		switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
		case GL_DEBUG_SEVERITY_LOW: return "LOW";
		case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
		case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
		default: return "No_Error";
		}
	}();

	logger("GLDebug").debug() << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << std::endl;
}
