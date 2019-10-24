/*!***************************************************************************************
\file       InputSystem.cpp
\author     Aaron Damyen
\date       7/3/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the InputSystem class member functions.
*****************************************************************************************/

//========Self Include==================================================================//
#include "InputSystem.hpp"
//========1st Party Includes============================================================//
#include "../Components/InputComponent.hpp"
#include "../GameObjectGatherer.hpp"
#include "../SystemManager.hpp"
#include "../Rendering/Windowing.hpp"
#include "../Messaging/Message.hpp"
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

bool inputSystem::keys[inputSystem::MAX_KEYS];
std::pair<double, double> inputSystem::mousePosition;
bool inputSystem::mouseButtons[MAXIMUM_MOUSE_BUTTONS];
actionType inputSystem::gamepadButtons[MAX_CONTROLLERS][XINPUT_BUTTON_OFFSET_MAX];
float inputSystem::gamepadAxis[MAX_CONTROLLERS][gpa_count];
actionType inputSystem::gamepadConnection[MAX_CONTROLLERS];
//int inputSystem::glfwToKeyMap[GLFW_KEY_LAST]; // TODO: Restore this?
inputSourceInterface * inputSystem::m_base_interface = nullptr;

/////================================================================================/////
/////     Functions / Data                                                           /////
/////================================================================================/////

void inputSystem::inputKeyboardListener(scancodeWrapper p_scancode, actionType p_action, unsigned int /*p_mods*/)
{
	switch(p_action)
	{
	case actionType::Pressed:
		keys[p_scancode.scancode] = true;
		break;
	case actionType::Released:
		keys[p_scancode.scancode] = false;
		break;
	case actionType::Repeat:
	default:
		// TODO: Log a warning here
		;
	}
}

void inputSystem::inputMousePosListener(double p_xpos, double p_ypos)
{
	mousePosition.first = static_cast<float>(p_xpos);
	mousePosition.second = static_cast<float>(p_ypos);
}

void inputSystem::inputGamepadButtonListener(unsigned short p_gamepadcode, actionType p_action, unsigned short p_id)
{
	gamepadButtons[p_id][p_gamepadcode] = p_action;
}

void inputSystem::inputGamepadAxisListener(const float* p_axisValues, size_t p_numAxis, unsigned short p_id)
{
	for(size_t i = 0; i < p_numAxis; ++i)
	{
		gamepadAxis[p_id][i] = p_axisValues[i];
	}
}

void inputSystem::inputConnectionStateListener(unsigned short p_id, actionType p_action)
{
	gamepadConnection[p_id] = p_action;
}

void inputSystem::addInputSource(inputSourceInterface* p_input_source_ptr)
{


	m_inputSources.push_back(p_input_source_ptr);
}

void inputSystem::onInitialize()
{
	// clean out memory
	std::memset(&keys, 0, sizeof(keys));
	std::memset(&mouseButtons, 0, sizeof(mouseButtons));

	// attach to the input source by setting up the callbacks
	// m_input_source_ptr->setKeyboardCallback(inputSystem::inputKeyboardListener);
	// m_input_source_ptr->setMousePosCallback(inputSystem::inputMousePosListener);
	for(auto& source : m_inputSources)
	{
		source->setKeyboardCallback(inputSystem::inputKeyboardListener);
		source->setMousePosCallback(inputSystem::inputMousePosListener);
		source->setConnectionStateCallbackFunction(inputSystem::inputConnectionStateListener);
		source->setGamepadAxisCallbackFunction(inputSystem::inputGamepadAxisListener);
		source->setGamepadButtonCallback(inputSystem::inputGamepadButtonListener);

	}


}

void inputSystem::onShutdown()
{
	// detach from the input source because our functions are about to go away.
	m_input_source_ptr->setKeyboardCallback(nullptr);
	m_input_source_ptr->setMousePosCallback(nullptr);
}

void inputSystem::onStartFrame()
{
	// get all the input components
	GameObjectFiltering::componentTypeSet l_input_component_pattern;
	l_input_component_pattern.setType(inputComponent::getType());
	std::vector<std::shared_ptr<gameObject>> l_list = getSystemManager()->getGameObjectGatherer()->getList(l_input_component_pattern);

	// iterate over all objects,
	for(std::shared_ptr<gameObject> & l_object : l_list)
	{
		// iterate over all input components on the object
		for (componentHandle<inputComponent>* l_component_ptr : l_object->getComponents<inputComponent>())
		{
			componentHandle<inputComponent>& l_component = *l_component_ptr;
			// if component's context is enabled
			// if()
			// {

			l_component->lock();

			// handle all change requests first
			if(!l_component->m_processing_requests.empty())
			{
				inputComponent::request req = l_component->m_processing_requests.front();
				l_component->m_processing_requests.pop();
				switch(req.type)
				{
				case inputComponent::request::req_type::AddLetterToName:
				{
					unsigned int l_code = m_input_source_ptr->mapLetterToScancode(req.letter).scancode;
					l_component->scancodeToNameMap[l_code] = req.name;
					l_component->nameToStatusMap[req.name] = false;
					break;
				}
				case inputComponent::request::req_type::AddMouseButtonToName:
				{
					unsigned int l_code = m_input_source_ptr->mapMouseButtonToScancode(req.mouseButton).scancode;
					l_component->scancodeToNameMap[l_code] = req.name;
					l_component->nameToStatusMap[req.name] = false;
					break;
				}
				case inputComponent::request::req_type::AddAxisToName:
				{
					l_component->axisToNameMap[req.axisCode] = req.name;
					l_component->nameToRangeMap[req.name] = 0;
					break;
				}
				case inputComponent::request::req_type::AddCursorToName:
				{
					l_component->cursorToNameMap[req.cursorCode] = req.name;
					l_component->nameToPositionMap[req.name] = std::make_pair(0, 0);
					break;
				}
				case inputComponent::request::req_type::RemoveButton:
				{
					l_component->nameToStatusMap.erase(req.name);
					for(auto l_key : l_component->scancodeToNameMap)
					{
						if(l_key.second == req.name)
						{
							l_component->scancodeToNameMap.erase(l_key.first);
							break;
						}
					}
					break;
				}
				case inputComponent::request::req_type::RemoveAxis:
				{
					l_component->nameToRangeMap.erase(req.name);
					for(auto l_key : l_component->axisToNameMap)
					{
						if (l_key.second == req.name)
						{
							l_component->axisToNameMap.erase(l_key.first);
							break;
						}
					}
					break;
				}
				case inputComponent::request::req_type::RemoveCursor:
				{
					l_component->nameToPositionMap.erase(req.name);
					for(auto l_key : l_component->cursorToNameMap)
					{
						if(l_key.second == req.name)
						{
							l_component->cursorToNameMap.erase(l_key.first);
							break;
						}
					}
					break;
				}
				case inputComponent::request::req_type::AddGamepadButtonToName:
					l_component->gamepadButtonToNameMap[req.gamepadCode] = req.name;
					l_component->gamepadNameToStatusMap[req.name] = actionType::Released;
					l_component->gamepadButtonNameToControllerMap[req.name] = req.gamepadID;
					break;
				case inputComponent::request::req_type::AddGamepadAxisToName:
					l_component->gamepadAxisToNameMap[req.gamepadAxisCode] = req.name;
					l_component->gamepadAxisNameToRangeMap[req.name] = 0.0f;
					l_component->gamepadAxisNameToControllerMap[req.name] = req.gamepadID;
					break;
				case inputComponent::request::req_type::RemoveGamepadButton:
					l_component->gamepadNameToStatusMap.erase(req.name);
					for(auto l_button : l_component->gamepadButtonToNameMap)
					{
						if(l_button.second == req.name)
						{
							l_component->gamepadButtonToNameMap.erase(l_button.first);
							break;
						}
					}
					l_component->gamepadButtonNameToControllerMap.erase(req.name);
					break;
				case inputComponent::request::req_type::RemoveGamepadAxis:
					l_component->gamepadAxisNameToRangeMap.erase(req.name);
					for(auto l_axis : l_component->gamepadAxisToNameMap)
					{
						if(l_axis.second == req.name)
						{
							l_component->gamepadAxisToNameMap.erase(l_axis.first);
							break;
						}
					}
					l_component->gamepadAxisNameToControllerMap.erase(req.name);
					break;
				case inputComponent::request::req_type::AddControllerState:
					l_component->controllerToNameMap[req.gamepadID] = req.name;
					l_component->controllerNameToStatusMap[req.name] = actionType::Released;
					break;
				case inputComponent::request::req_type::RemoveControllerState:
					l_component->controllerNameToStatusMap.erase(req.name);
					for(auto l_controller : l_component->controllerToNameMap)
					{
						if(l_controller.second == req.name)
						{
							l_component->controllerToNameMap.erase(l_controller.first);
						}
					}

					break;
					default: ;
				}
			}

			// iterate over all keycodes on the component
			for(auto & l_keycode_pair : l_component->scancodeToNameMap)
			{
				if (!isMouseButton(l_keycode_pair.first))
				{
					// calculate the status of the keycode
					bool l_is_active = getKey(l_keycode_pair.first);
					// set the status
					l_component->nameToStatusMap[l_keycode_pair.second] = l_is_active;
				}
				else
				{
					// get the mouse button status
					l_component->nameToStatusMap[l_keycode_pair.second] = getMouseButton(l_keycode_pair.first);
				}
			}

			// iterate over all cursors on the component
			for(auto & l_cursor_pair : l_component->cursorToNameMap)
			{
				if (l_cursor_pair.first == inputComponent::Cursor::Default_Mouse)
				{
					// set the position
					l_component->nameToPositionMap[l_cursor_pair.second] = mousePosition;
				}
			}
			l_component->unlock();

			//gamepad buttons
			for(auto& l_gamepadButton : l_component->gamepadButtonToNameMap)
			{
				unsigned int id = l_component->gamepadButtonNameToControllerMap[l_gamepadButton.second];
				actionType state = getGamepadButton(l_gamepadButton.first, id);
				l_component->gamepadNameToStatusMap[l_gamepadButton.second] = state;
			}
			//gamepad axis
			for(auto& l_gamepadAxis : l_component->gamepadAxisToNameMap)
			{
				unsigned int id = l_component->gamepadAxisNameToControllerMap[l_gamepadAxis.second];
				float state = getGamepadAxis(l_gamepadAxis.first, id);
				l_component->gamepadAxisNameToRangeMap[l_gamepadAxis.second] = state;
			}
			//controller state
			for(auto& controller : l_component->controllerToNameMap)
			{
				l_component->controllerNameToStatusMap[controller.second] = getControllerState(controller.first);
			}



			// }
			// else  // else, clear all statuses
			// {
			// }
		}
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

bool inputSystem::getKey(int p_key) const
{
	return keys[p_key];
}

bool inputSystem::isMouseButton(int p_key) const
{
	if (p_key == inputComponent::Keys::Mouse_Left
	 || p_key == inputComponent::Keys::Mouse_Middle
	 || p_key == inputComponent::Keys::Mouse_Right)
		return true;
	return false;
}

bool inputSystem::getMouseButton(int p_button) const
{
	return mouseButtons[p_button];
}

actionType inputSystem::getGamepadButton(unsigned p_button, unsigned p_id)
{
	return gamepadButtons[p_id][p_button];
}

float inputSystem::getGamepadAxis(unsigned p_axis, unsigned p_id)
{
	return gamepadAxis[p_id][p_axis];
}

actionType inputSystem::getControllerState(unsigned p_id)
{
	return gamepadConnection[p_id];
}


/*======================================================================================/
/         Other                                                                         /
//======================================================================================*/

// namespace <namespace name>


