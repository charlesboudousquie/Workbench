/*!***************************************************************************************
\file       InputComponent.cpp
\author     Aaron Damyen
\date       7/3/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the InputComponent class member functions.
*****************************************************************************************/

//========Self Include==================================================================//
#include "InputComponent.hpp"
#include "../Input/InputSystem.hpp"
#include <algorithm>
#include "../SceneManagement/GameObject.hpp"
//========1st Party Includes============================================================//
//========3rd Party Includes============================================================//
//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//

/////////========================================================================/////////
///////// Namespace                                                              /////////
/////////========================================================================/////////

////////==========================================================================////////
////////  Struct / Class                                                          ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: classExample
\brief   This is a stub class to be used as a template / example for class formating.
*****************************************************************************************/

///////============================================================================///////
///////   Public/Protected/Private                                                 ///////
///////============================================================================///////

//////==============================================================================//////
//////    (Non-)Static                                                              //////
//////==============================================================================//////

/////================================================================================/////
/////     Functions / Data                                                           /////
/////================================================================================/////

void inputComponent::addButtonMapping(unsigned int p_letter, std::string p_name)
{
	// make sure letter is uppercase
	if (p_letter >= 'a' && p_letter <= 'z')
		p_letter = p_letter - 'a' + 'A';

	lock();
	processRequest(request::makeAddLetterToName(p_letter, p_name));
	unlock();
}

void inputComponent::addButtonMapping(inputComponent::Keys p_key, std::string p_name)
{
	if(p_key == inputComponent::Keys::Mouse_Left || p_key==inputComponent::Keys::Mouse_Middle
		|| p_key == inputComponent::Keys::Mouse_Right)
	{
		lock();
		processRequest(request::makeAddMouseButtonToName(p_key, p_name));
		unlock();
	}
	else
	{
		addButtonMapping(static_cast<unsigned int>(p_key), p_name);
	}
}

void inputComponent::removeButtonMapping(std::string p_name)
{
	lock();
	processRequest(request::makeRemoveButton(p_name));
	unlock();
}

void inputComponent::addAxisMapping(int p_axiscode, std::string p_name)
{
	lock();
	processRequest(request::makeAddAxisToName(p_axiscode, p_name));
	unlock();
}

void inputComponent::removeAxisMapping(std::string p_name)
{
	lock();
	processRequest(request::makeRemoveAxis(p_name));
	unlock();
}

void inputComponent::addCursorMapping(Cursor p_cursorcode, std::string p_name)
{
	lock();
	processRequest(request::makeAddCursorToName(p_cursorcode, p_name));
	unlock();
}

void inputComponent::removeCursorMapping(std::string p_name)
{
	lock();
	processRequest(request::makeRemoveCursor(p_name));
	unlock();
}

void inputComponent::addGamepadButtonMapping(unsigned p_button, unsigned p_id, const std::string& p_name)
{
	lock();
	processRequest(request::makeAddGamepadButton(p_button, p_id, p_name));
	unlock();
}

void inputComponent::removeGamepadButtonMapping(const std::string& p_name)
{
	lock();
	processRequest(request::makeRemoveGamepadButton(p_name));
	unlock();
}

void inputComponent::addGamepadAxisMapping(unsigned p_axis, unsigned p_id, const std::string& p_name)
{
	lock();
	processRequest(request::makeAddGamepadAxis(p_axis, p_id, p_name));
	unlock();
}

void inputComponent::removeGamepadAxisMapping(const std::string& p_name)
{
	lock();
	processRequest(request::makeRemoveGamepadAxis(p_name));
	unlock();
}

void inputComponent::addControllerStateMapping(unsigned p_id, const std::string& p_name)
{
	lock();
	processRequest(request::makeAddControllerState(p_id, p_name));
	unlock();
}

void inputComponent::removeControllerStateMapping(const std::string& p_name)
{
	lock();
	processRequest(request::makeRemoveControllerState(p_name));
	unlock();
}

void inputComponent::clearAllMaps()
{
	scancodeToNameMap.clear();
	nameToStatusMap.clear();
	axisToNameMap.clear();
	nameToRangeMap.clear();
	cursorToNameMap.clear();
	nameToPositionMap.clear();
	gamepadButtonToNameMap.clear();
	gamepadNameToStatusMap.clear();
	gamepadButtonNameToControllerMap.clear();
	gamepadAxisToNameMap.clear();
	gamepadAxisNameToRangeMap.clear();
	gamepadAxisNameToControllerMap.clear();
	controllerToNameMap.clear();
	controllerNameToStatusMap.clear();
}

void inputComponent::lock()
{
	m_write_mutex.lock();
}

void inputComponent::unlock()
{
	m_write_mutex.unlock();
}

void inputComponent::processRequest(const request& req)
{

	switch (req.type)
	{
	case inputComponent::request::req_type::AddLetterToName:
	{
		unsigned int l_code = inputSystem::m_base_interface->mapLetterToScancode(req.letter).scancode;
		scancodeToNameMap[l_code] = req.name;
		nameToStatusMap[req.name] = false;
		break;
	}
	case inputComponent::request::req_type::AddMouseButtonToName:
	{
		unsigned int l_code = inputSystem::m_base_interface->mapMouseButtonToScancode(req.mouseButton).scancode;
		scancodeToNameMap[l_code] = req.name;
		nameToStatusMap[req.name] = false;
		break;
	}
	case inputComponent::request::req_type::AddAxisToName:
	{
		axisToNameMap[req.axisCode] = req.name;
		nameToRangeMap[req.name] = 0;
		break;
	}
	case inputComponent::request::req_type::AddCursorToName:
	{
		cursorToNameMap[req.cursorCode] = req.name;
		nameToPositionMap[req.name] = std::make_pair(0, 0);
		break;
	}
	case inputComponent::request::req_type::RemoveButton:
	{
		nameToStatusMap.erase(req.name);
		for (auto l_key : scancodeToNameMap)
		{
			if (l_key.second == req.name)
			{
				scancodeToNameMap.erase(l_key.first);
				break;
			}
		}
		break;
	}
	case inputComponent::request::req_type::RemoveAxis:
	{
		nameToRangeMap.erase(req.name);
		for (auto l_key : axisToNameMap)
		{
			if (l_key.second == req.name)
			{
				axisToNameMap.erase(l_key.first);
				break;
			}
		}
		break;
	}
	case inputComponent::request::req_type::RemoveCursor:
	{
		nameToPositionMap.erase(req.name);
		for (auto l_key : cursorToNameMap)
		{
			if (l_key.second == req.name)
			{
				cursorToNameMap.erase(l_key.first);
				break;
			}
		}
		break;
	}
	case inputComponent::request::req_type::AddGamepadButtonToName:
		gamepadButtonToNameMap[req.gamepadCode] = req.name;
		gamepadNameToStatusMap[req.name] = actionType::Released;
		gamepadButtonNameToControllerMap[req.name] = req.gamepadID;
		break;
	case inputComponent::request::req_type::AddGamepadAxisToName:
		gamepadAxisToNameMap[req.gamepadAxisCode] = req.name;
		gamepadAxisNameToRangeMap[req.name] = 0.0f;
		gamepadAxisNameToControllerMap[req.name] = req.gamepadID;
		break;
	case inputComponent::request::req_type::RemoveGamepadButton:
		gamepadNameToStatusMap.erase(req.name);
		for (auto l_button : gamepadButtonToNameMap)
		{
			if (l_button.second == req.name)
			{
				gamepadButtonToNameMap.erase(l_button.first);
				break;
			}
		}
		gamepadButtonNameToControllerMap.erase(req.name);
		break;
	case inputComponent::request::req_type::RemoveGamepadAxis:
		gamepadAxisNameToRangeMap.erase(req.name);
		for (auto l_axis : gamepadAxisToNameMap)
		{
			if (l_axis.second == req.name)
			{
				gamepadAxisToNameMap.erase(l_axis.first);
				break;
			}
		}
		gamepadAxisNameToControllerMap.erase(req.name);
		break;
	case inputComponent::request::req_type::AddControllerState:
		controllerToNameMap[req.gamepadID] = req.name;
		controllerNameToStatusMap[req.name] = actionType::Released;
		break;
	case inputComponent::request::req_type::RemoveControllerState:
		controllerNameToStatusMap.erase(req.name);
		for (auto l_controller : controllerToNameMap)
		{
			if (l_controller.second == req.name)
			{
				controllerToNameMap.erase(l_controller.first);
			}
		}

		break;
	default:;
	}
}


////==================================================================================////
////      (Non-)Configurable                                                          ////
////==================================================================================////

////==================================================================================////
////      Overridden/(Non-/Pure-)Virtual                                              ////
////==================================================================================////

//======================================================================================//
//        (Conversion )Constructor / Operators / Getters & Setters / Helper Destructor  //
//======================================================================================//

inputComponent::inputComponent(typeRT& p_type_data)
{
	updateFromTypeRT(p_type_data);
}

inputComponent::~inputComponent()
{
	clearAllMaps();
}

typeRT inputComponent::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());

	typeRT contextRT = typeRT("context", context);
	result.insertMember(contextRT);

	std::vector<typeRT> l_map;
	for(auto l_iter : scancodeToNameMap)
	{
		typeRT l_entry;
		l_entry.setTypeName("scancodeToNameMapEntry");
		l_entry.insertMember(typeRT("scancode", l_iter.first));
		l_entry.insertMember(typeRT("name", l_iter.second));
		l_map.push_back(l_entry);
	}
	typeRT scancodeToNameMapRT = typeRT("scancodeToNameMap", l_map);
	result.insertMember(scancodeToNameMapRT);

	l_map.clear();
	for(auto l_iter : axisToNameMap)
	{
		typeRT l_entry;
		l_entry.setTypeName("axisToNameMapEntry");
		l_entry.insertMember(typeRT("axis", l_iter.first));
		l_entry.insertMember(typeRT("name", l_iter.second));
		l_map.push_back(l_entry);
	}
	typeRT axisToNameMapRT = typeRT("axisToNameMap", l_map);
	result.insertMember(axisToNameMapRT);

	l_map.clear();
	for(auto l_iter : cursorToNameMap)
	{
		typeRT l_entry;
		l_entry.setTypeName("cursorToNameMapEntry");
		l_entry.insertMember(typeRT("cursor", l_iter.first));
		l_entry.insertMember(typeRT("name", l_iter.second));
		l_map.push_back(l_entry);
	}
	typeRT cursorToNameMapRT = typeRT("cursorToNameMap", l_map);
	result.insertMember(cursorToNameMapRT);

	l_map.clear();
	for(auto l_iter : gamepadButtonToNameMap)
	{
		typeRT l_entry;
		l_entry.setTypeName("gamepadButtonToNameMapEntry");
		l_entry.insertMember(typeRT("gamepadButton", l_iter.first));
		l_entry.insertMember(typeRT("name", l_iter.second));
		unsigned int controllerID = gamepadButtonNameToControllerMap.at(l_iter.second);
		l_entry.insertMember(typeRT("controllerID", controllerID));
		l_map.push_back(l_entry);
	}
	typeRT gamepadButtonToNameMapRT = typeRT("gamepadButtonToNameMap", l_map);
	result.insertMember(gamepadButtonToNameMapRT);

	l_map.clear();
	for(auto l_iter : gamepadAxisToNameMap)
	{
		typeRT l_entry;
		l_entry.setTypeName("gamepadAxisToNameMapEntry");
		l_entry.insertMember(typeRT("gamepadAxis", l_iter.first));
		l_entry.insertMember(typeRT("name", l_iter.second));
		unsigned int controllerID = gamepadAxisNameToControllerMap.at(l_iter.second);
		l_entry.insertMember(typeRT("controllerID", controllerID));
		l_map.push_back(l_entry);
	}
	typeRT gamepadAxisToNameMapRT = typeRT("gamepadAxisToNameMap", l_map);
	result.insertMember(gamepadAxisToNameMapRT);

	l_map.clear();
	for(auto l_iter : controllerToNameMap)
	{
		typeRT l_entry;
		l_entry.setTypeName("controllerToNameEntry");
		l_entry.insertMember(typeRT("controllerID", l_iter.first));
		l_entry.insertMember(typeRT("name", l_iter.second));
		l_map.push_back(l_entry);
	}
	typeRT controllerToNameMapRT = typeRT("controllerToNameMap", l_map);
	result.insertMember(controllerToNameMapRT);

	return result;
}

void inputComponent::updateFromTypeRT(typeRT& p_type)
{
	clearAllMaps();
	auto l_members = p_type.members();

	if(l_members.find("context") != l_members.end())
	{
		context = p_type.member("context").getString();
	}
	if (l_members.find("scancodeToNameMap") != l_members.end())
	{
		for(auto l_entry : p_type.member("scancodeToNameMap").array())
		{
			unsigned int scancode = l_entry.member("scancode").getUInt();
			std::string name = l_entry.member("name").getString();

			addButtonMapping(scancode, name);
			// scancodeToNameMap.insert_or_assign(scancode, name);
			// nameToStatusMap.insert_or_assign(name, false);
		}
	}
	if(l_members.find("axisToNameMap") != l_members.end())
	{
		for(auto l_entry : p_type.member("axisToNameMap").array())
		{
			int axis = l_entry.member("axis").getInt();
			std::string name = l_entry.member("name").getString();
			addAxisMapping(axis, name);
			// axisToNameMap.insert_or_assign(axis, name);
			//nameToRangeMap.insert_or_assign(name, 0);
		}
	}
	if(l_members.find("cursorToNameMap") != l_members.end())
	{
		for(auto l_entry : p_type.member("cursorToNameMap").array())
		{
			Cursor cursor = static_cast<Cursor>(l_entry.member("cursor").getInt());
			std::string name = l_entry.member("name").getString();
			addCursorMapping(cursor, name);
			// cursorToNameMap.insert_or_assign(cursor, name);
			//nameToPositionMap.insert_or_assign(name, std::make_pair(0.0, 0.0));
		}
	}
	if(l_members.find("gamepadButtonToNameMap") != l_members.end())
	{
		for(auto l_entry : p_type.member("gamepadButtonToNameMap").array())
		{
			unsigned int gamepadButton = l_entry.member("gamepadButton").getUInt();
			std::string name = l_entry.member("name").getString();
			unsigned int controllerID = l_entry.member("controllerID").getUInt();
			// gamepadButtonToNameMap.insert_or_assign(gamepadButton, name);
			//gamepadNameToStatusMap.insert_or_assign(...);
			// gamepadButtonNameToControllerMap.insert_or_assign(name, controllerID);
			addGamepadButtonMapping(gamepadButton, controllerID, name);
		}
	}
	if(l_members.find("gamepadAxisToNameMap") != l_members.end())
	{
		for(auto l_entry : p_type.member("gamepadAxisToNameMap").array())
		{
			unsigned int gamepadAxis = l_entry.member("gamepadAxis").getUInt();
			std::string name = l_entry.member("name").getString();
			unsigned int controllerID = l_entry.member("controllerID").getUInt();
			// gamepadAxisToNameMap.insert_or_assign(gamepadAxis, name);
			//gamepadAxisNameToRangeMap.insert_or_assign(...);
			// gamepadAxisNameToControllerMap.insert_or_assign(name, controllerID);
			addGamepadAxisMapping(gamepadAxis, controllerID, name);
		}
	}
	if(l_members.find("controllerToNameMap") != l_members.end())
	{
		for(auto l_entry : p_type.member("controllerToNameMap").array())
		{
			unsigned int controller = l_entry.member("controllerID").getUInt();
			std::string name = l_entry.member("name").getString();
			addControllerStateMapping(controller, name);
		}
	}
}

bool inputComponent::getButtonState(std::string const& p_name)
{
	// if (nameToStatusMap.find(p_name) == nameToStatusMap.end())
	// 	return false;
 //    bool l_result = nameToStatusMap[p_name];
 //    return l_result;
	auto key = nameToStatusMap.find(p_name);
	auto gpb = gamepadNameToStatusMap.find(p_name);
	bool key_down = false;
	bool gpb_down = false;
	if(key != nameToStatusMap.end())
	{
		key_down = key->second;
	}
	else
	{

	}
	if(gpb != gamepadNameToStatusMap.end())
	{
		gpb_down = (gpb->second == actionType::Pressed || gpb->second == actionType::Repeat);
	}

	return gpb_down || key_down;
}

float inputComponent::getAxisState(std::string const & p_name)
{
	// if (nameToRangeMap.find(p_name) == nameToRangeMap.end())
	// 	return 0;
 //    return nameToRangeMap[p_name];
	auto gen = nameToRangeMap.find(p_name);
	auto gpa = gamepadAxisNameToRangeMap.find(p_name);
	float gen_axis = 0;
	float gpa_axis = 0;
	if(gen != nameToRangeMap.end())
	{
		gen_axis = gen->second;
	}
	if(gpa != gamepadAxisNameToRangeMap.end())
	{
		gpa_axis = gpa->second;
	}
	return std::max<float>(gen_axis, gpa_axis);
}

bool inputComponent::getConnectionState(std::string const& p_name)
{
	auto state = controllerNameToStatusMap.find(p_name);
	if(state != controllerNameToStatusMap.end())
	{
		return state->second == actionType::Repeat || state->second == actionType::Pressed;
	}
	return false;
}

std::pair<double, double> inputComponent::getCursorState(std::string const& p_name)
{
	if (nameToPositionMap.find(p_name) == nameToPositionMap.end())
		return std::pair<double, double>(0, 0);
    return nameToPositionMap[p_name];
}

void inputComponent::setContext(std::string p_context)
{
    context = p_context;
}

std::string inputComponent::getContext() const
{
    return context;
}


/*======================================================================================/
/         Other                                                                         /
//======================================================================================*/

// namespace <namespace name>


