/*!***************************************************************************************
\file       Logger.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/

//========1st Party Includes============================================================//
#include "Logger.hpp"

//========3rd Party Includes============================================================//
#ifdef _WIN32
#include <Windows.h>
#else
#include <locale>
#include <codecvt>
#include <syslog.h>
#endif

//========Forward Deceleration==========================================================//

/////////========================================================================/////////
///////// Namespace                                                              /////////
/////////========================================================================/////////

////////==========================================================================////////
////////  Struct / Class                                                          ////////
////////==========================================================================////////
logger::loggerProxy::~loggerProxy()
{
    m_logger->message(level, this->str().c_str());
}

///////============================================================================///////
///////   Public/Protected/Private                                                 ///////
///////============================================================================///////

//////==============================================================================//////
//////    (Non-)Static                                                              //////
//////==============================================================================//////


logger::logger(std::string p_system_designator)
	: m_tag{p_system_designator.begin(),p_system_designator.end()}
{
#ifndef _WIN32
	openlog(p_system_designator.c_str(), LOG_CONS | LOG_PERROR | LOG_PID, LOG_USER);
#endif
}

logger::loggerProxy logger::info()
{
    return loggerProxy(this, Level::LLEVEL_INFO);
}

logger::loggerProxy logger::debug()
{
    return loggerProxy(this, Level::LLEVEL_DEBUG);
}

logger::loggerProxy logger::warn()
{
    return loggerProxy(this, Level::LLEVEL_WARN);
}

logger::loggerProxy logger::error()
{
    return loggerProxy(this, Level::LLEVEL_ERROR);
}

void logger::info(std::string p_message)
{
    logger::message(Level::LLEVEL_ERROR, p_message);
}

void logger::debug(std::string p_message)
{
    logger::message(Level::LLEVEL_DEBUG, p_message);
}

void logger::warn(std::string p_message)
{
    logger::message(Level::LLEVEL_WARN, p_message);
}
void logger::error(std::string p_message)
{
    logger::message(Level::LLEVEL_ERROR, p_message);
}

void logger::message(Level p_level, std::string p_message)
{
#ifdef _WIN32
    // convert the std::string to a wide string (std::wstring)
    // std::wstring l_wide_level = to_wstring(p_level);
    std::wstring l_wide_message = std::wstring(p_message.begin(), p_message.end());
    message(p_level, l_wide_message);
#else
	syslog(to_int(p_level), "%s", p_message.c_str());
#endif
}

void logger::message(Level p_level, std::wstring p_message)
{
    std::wstringstream l_stream;
    l_stream << m_tag << ": " << to_wstring(p_level) << " - " << p_message << std::endl;

#ifdef _WIN32
    OutputDebugStringW(l_stream.str().c_str());
#else
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> l_converter;
    std::string l_s = l_converter.to_bytes(l_stream.str());

    message(p_level, l_s.c_str());
#endif
}

int logger::to_int(Level p_level)
{
#ifndef _WIN32
	switch(p_level)
	{
		case Level::LLEVEL_INFO:
			return LOG_INFO;
		case Level::LLEVEL_WARN:
			return LOG_WARNING;
		case Level::LLEVEL_ERROR:
			return LOG_ERR;
		case Level::LLEVEL_DEBUG:
		case Level::LLEVEL_NONE:
		default:
			return LOG_DEBUG;
	}
#else
	return 0;
#endif
}

std::string logger::to_string(Level p_level)
{
	switch(p_level)
	{
		case Level::LLEVEL_INFO:
			return "Info";
		case Level::LLEVEL_WARN:
			return "Warn";
		case Level::LLEVEL_ERROR:
			return "Error";
		case Level::LLEVEL_DEBUG:
		case Level::LLEVEL_NONE:
		default:
			return "Debug";
	}
}

std::wstring logger::to_wstring(Level p_level)
{
	switch(p_level)
	{
		case Level::LLEVEL_INFO:
			return L"Info";
		case Level::LLEVEL_WARN:
			return L"Warn";
		case Level::LLEVEL_ERROR:
			return L"Error";
		case Level::LLEVEL_DEBUG:
		case Level::LLEVEL_NONE:
		default:
			return L"Debug";
	}
}

/////================================================================================/////
/////     Functions / Data                                                           /////
/////================================================================================/////

////==================================================================================////
////      (Non-)Configurable                                                          ////
////==================================================================================////

////==================================================================================////
////      Oveided/(Non-/Pure-)Virtual                                                 ////
////==================================================================================////

//======================================================================================//
//        Helper / Constructor / Destructor                                            ///
//======================================================================================//

//======================================================================================//
//        Other                                                                         //
//======================================================================================/
