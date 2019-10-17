/*!***************************************************************************************
\file       InputSystem.hpp
\author     Aaron Damyen
\date       6/10/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Defines the Input system for converting all input forms into actions within the engine.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "../SystemBase.hpp"
#include "InputSourceInterface.hpp"
#include "GamepadControlDefines.hpp"
#include "GamepadInputAdapter.hpp"

#include <list>

//========3rd Party Includes============================================================//

class inputComponent;

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////



/*!***************************************************************************************
\par class: inputSystem
\brief   Definition of the input system class for wrangling input commands
*****************************************************************************************/
class inputSystem final : public systemBase
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:

	inputSystem(inputSourceInterface * p_input_source_ptr) 
		: m_input_source_ptr{ p_input_source_ptr }
	{
		m_base_interface = p_input_source_ptr;
		m_inputSources.push_back(p_input_source_ptr);

	}

	/*!***************************************************************************************
	\brief  Converts a letter (or other 'virtual key') into the appropriate scancodeWrapper
	\param p_letter - The letter to convert.  IMPORTANT: On Windows, use uppercase letters!
	\return unsigned int - The scancode equivalent of the parameter letter
	*****************************************************************************************/
	unsigned int mapLetterToScancode(unsigned int p_letter)
	{
		return m_input_source_ptr->mapLetterToScancode(p_letter).scancode;
	}

	void addInputSource(inputSourceInterface *p_input_source_ptr);

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	static inputSourceInterface * m_base_interface; //!< the design has collapsed, its just got to be global

	/*!***************************************************************************************
	\brief  retrieves the name of this system. There are two versions for the same functionality
			so that we can get the name from the class as well as an instance
	\return the name of the s
	*****************************************************************************************/
	static const std::string& getName() { static const std::string n("inputSystem"); return n; }
	virtual const std::string& name() const override { return getName(); }

	static const bool * GetAllKeys() { return keys; }

	/*!***************************************************************************************
	\brief  Static constant for the maximum number of mouse button this system can handle
	*****************************************************************************************/
	static constexpr unsigned int MAXIMUM_MOUSE_BUTTONS = 5;

	/*!***************************************************************************************
	\par class: messageCodes
	\brief  Inner class used to group the constants for message codes
	*****************************************************************************************/
	class messageCodes
	{
	public:
		static constexpr unsigned int convertLetterToScancode = 1;
		static constexpr unsigned int convertMouseButtonToScancode = 2;
	};



	////////========================================================================////////
	////////   Protected                                                            ////////
	////////========================================================================////////
protected:

	/*!***************************************************************************************
	\brief  Handles the initialize event from the system manager to allocate resources and
	        connect to the input source
	*****************************************************************************************/
	void onInitialize() override;

	/*!***************************************************************************************
	\brief  Handles the start frame event from the system manager to begin processing input
	*****************************************************************************************/
	void onStartFrame() override;

	/*!***************************************************************************************
	\brief  Handles the shutdown event from the system manager to free resources and disconnect
	        from the input source
	*****************************************************************************************/
	void onShutdown() override;

	///*!***************************************************************************************
	//\brief  Handles the message notification event from the system manager to do something
	//        for another system/object/process.
	//\param message - The structure containing all information about the message
	//*****************************************************************************************/
	//void onHandleMessage(message & p_message) override;

	//////==============================================================================//////
	//////    Static                                                              //////
	//////==============================================================================//////



	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
private:

	/*!***************************************************************************************
	\brief  Retrieves if a specific key is being pressed
	\param p_key - the identifier for the key to check
	\return bool - Returns true if the key is currently pressed, false otherwise
	*****************************************************************************************/
	bool getKey(int p_key) const;

	/*!***************************************************************************************
	\brief  Retrieves if a key is actually a mouse button
	\param p_key - the identifier of the key to check
	\return bool - Returns true if the key is a mouse button, false otherwise
	*****************************************************************************************/
	bool isMouseButton(int p_key) const;

	/*!***************************************************************************************
	\brief  Retrieves if a specific mouse button is being pressed
	\param p_button - the identifier for the mouse button to check
	\return bool - Returns true if the mouse button is currently pressed, false otherwise
	*****************************************************************************************/
	bool getMouseButton(int p_button) const;

	actionType getGamepadButton(unsigned int p_button, unsigned int p_id);

	float getGamepadAxis(unsigned int p_axis, unsigned int p_id);

	actionType getControllerState(unsigned int p_id);

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	inputSourceInterface * m_input_source_ptr; //!< Reference to the input source

	std::list<inputSourceInterface*> m_inputSources;


	/*!***************************************************************************************
	\brief  Handles the callback from the input source for keyboard input.  Note that only the
	        scancodeWrapper is retrieved.  Virtual keys should be translated into scancodes before
	        being used.
	\param p_scancode - the keyboard scancode this event is for
	\param p_action - the action that caused this event
	\param p_mods - modifier keys currently pressed
	*****************************************************************************************/
	static void inputKeyboardListener(scancodeWrapper p_scancode, actionType p_action, unsigned int p_mods);

	/*!***************************************************************************************
	\brief  Handles the callback from the input source for mouse position changes
	\param p_xpos - the current x position of the mouse
	\param p_ypos - the current y position of the mouse
	*****************************************************************************************/
	static void inputMousePosListener(double p_xpos, double p_ypos);

	static void inputGamepadButtonListener(unsigned short p_gamepadcode, actionType p_action, unsigned short p_id);

	static void inputGamepadAxisListener(const float* p_axisValues, size_t p_numAxis, unsigned short p_id);

	static void inputConnectionStateListener(unsigned short p_id, actionType p_action);

	static constexpr int MAX_KEYS = 1024;

	static bool keys[MAX_KEYS]; //!< current status of known buttons
	static std::pair<double, double> mousePosition; //!< current status of the mouse
	static bool mouseButtons[MAXIMUM_MOUSE_BUTTONS]; //!< current status of mouse buttons
	static actionType gamepadButtons[MAX_CONTROLLERS][XINPUT_BUTTON_OFFSET_MAX];
	static float gamepadAxis[MAX_CONTROLLERS][gpa_count];
	static actionType gamepadConnection[MAX_CONTROLLERS];



};


