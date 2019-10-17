/*!***************************************************************************************
\file       InputComponent.hpp
\author     Aaron Damyen
\date       6/10/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Definition of the input component.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//

//========3rd Party Includes============================================================//
#include <string>
#include <unordered_map>
#include <queue>
#include "../SystemManager.hpp"
#include <InputSourceInterface.hpp>
#include <Component.hpp>

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: inputComponent
\brief   Defines a component for translating input commands into actions.
*****************************************************************************************/
class inputComponent final : public componentCRTP<inputComponent>
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:

	/*!***************************************************************************************
	\par enum: Keys
	\brief  Defines all the keys that the input component understands and can monitor.  This 
	        includes mouse buttons.
	*****************************************************************************************/
	enum Keys
	{
		Mouse_Left = 0, Mouse_Middle = 2, Mouse_Right = 1,

		A = 'A', B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		/*ESC, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,*/
		Grave = '`', Digit_1 = '1', Digit_2, Digit_3, Digit_4, Digit_5, Digit_6, Digit_7, Digit_8, Digit_9, Digit_0 = '0',
		Hyphen = '-', Equal = '=', /*Backspace, */
		Tab = '\t', Open_Bracket = '[', Close_Bracket = ']', Backslash = '\\',
		Semicolon = ';', Apostrophe='\'', Enter='\n', /*
	    Shift_Left,*/ Comma=',', Period='.', Slash='/', /*Shift_Right,
	    Control_Left, Alt_Left,*/ Space, /*Alt_Right, Control_Right,
	    Insert, Home, Page_Up, Delete, End, Page_Down, */
	    Up, Left, Down, Right,
	    /*Num_Slash, Num_Asterisk, Num_Minus, Num_Plus, Num_Enter,
	    Num_0, Num_1, Num_2, Num_3, Num_4, Num_5, Num_6, Num_7, Num_8, Num_9, Num_Period,*/
	
	};

	/*!***************************************************************************************
	\par enum: Cursor
	\brief  Defines all the cursors that the input component understands and can monitor.
	*****************************************************************************************/
	enum Cursor { Default_Mouse = 0 };
	
	/*!***************************************************************************************
	\brief  Creates the input component object and initializes it.
	*****************************************************************************************/
	inputComponent() = default;

	/*!***************************************************************************************
	\brief  Constructs this object and sets the initial values from the typeRT data.
	\param p_type_data - the typeRT data structure with the initial values
	*****************************************************************************************/
	inputComponent(typeRT & p_type_data);

	/*!***************************************************************************************
	\brief  Destroys the input component, releasing all allocated resources.
	*****************************************************************************************/
	virtual ~inputComponent();
	
	/*!***************************************************************************************
	\brief  Converts this object into a typeRT structure
	\return typeRT - a typeRT structure with all serializable values for this object
	*****************************************************************************************/
	typeRT toTypeRT() const override;

	/*!***************************************************************************************
	\brief  Updates the values of this object to the values in the typeRT structure
	\param p_type - a typeRT structure with all serializable values for this object
	*****************************************************************************************/
	void updateFromTypeRT(typeRT& p_type) override;

	/*!***************************************************************************************
	\brief  Retrieves the state of a button mapped to a name.
	\param p_name - The name of the button to retrieve.
	\return bool - Returns true if the button is pressed, false otherwise.
	*****************************************************************************************/
	bool getButtonState(std::string const & p_name);

	/*!***************************************************************************************
	\brief  Retrieves the state of an axis mapped to a name.
	\param p_name - The name of the axis to retrieve.
	\return double - Returns the value of the specified axis.
	*****************************************************************************************/
	float getAxisState(std::string const & p_name);

	/*!***************************************************************************************
	\brief Get the connection status of a given named controller
	\param p_name name of connection state 
	\return true if controller connected, false otherwise
	*****************************************************************************************/
	bool getConnectionState(std::string const& p_name);

	/*!***************************************************************************************
	\brief  Retrieves the state of a cursor mapped to a name.
	\param p_name - The name of the cursor to retrieve.
	\return std::pair<double, double> - Returns the (x,y) coordinates of the cursor.
	*****************************************************************************************/
	std::pair<double, double> getCursorState(std::string const & p_name);

	/*!***************************************************************************************
	\brief  Assigns this instance to a specific context.
	\param p_context - the context to which this instance is assigned.
	*****************************************************************************************/
	void setContext(std::string p_context);

	/*!***************************************************************************************
	\brief  Retrieves the currently assign context.
	\return std::string - The context currently assigned.
	*****************************************************************************************/
	std::string getContext() const;

	/*!***************************************************************************************
	\brief  Associates a name with a lettered button on the keyboard.
	\param p_letter - The letter for the button that is the domain for the association.
	\param p_name - The name that is the range for the association.
	*****************************************************************************************/
	void addButtonMapping(unsigned int p_letter, std::string p_name);

	void addButtonMapping(inputComponent::Keys p_key, std::string p_name);

	/*!***************************************************************************************
	\brief  Removes all button associations for a name.
	\param p_name - The name of the range of the associations to remove.
	*****************************************************************************************/
	void removeButtonMapping(std::string p_name);

	/*!***************************************************************************************
	\brief  Associates a name with an axis.
	\param p_axiscode - The axis that is the domain for the association.
	\param p_name - The name that is the range for the association.
	*****************************************************************************************/
	void addAxisMapping(int p_axiscode, std::string p_name);

	/*!***************************************************************************************
	\brief  Removes all axis associations for a name.
	\param p_name - The name of the range of the associations to remove.
	*****************************************************************************************/
	void removeAxisMapping(std::string p_name);

	/*!***************************************************************************************
	\brief  Associates a name with a cursor.
	\param p_cursorcode - The cursor that is the domain for the association.
	\param p_name - The name that is the range for the assocation.
	*****************************************************************************************/
	void addCursorMapping(Cursor p_cursorcode, std::string p_name);

	/*!***************************************************************************************
	\brief  Removes all cursor assocations for a name.
	\param p_name - The name of the range of the associations to remove.
	*****************************************************************************************/
	void removeCursorMapping(std::string p_name);

	void addGamepadButtonMapping(unsigned int p_button, unsigned int p_id, const std::string& p_name);

	void removeGamepadButtonMapping(const std::string& p_name);

	void addGamepadAxisMapping(unsigned int p_axis, unsigned int p_id, const std::string& p_name);

	void removeGamepadAxisMapping(const std::string& p_name);

	void addControllerStateMapping(unsigned int p_id, const std::string& p_name);

	void removeControllerStateMapping(const std::string& p_name);

	void clearAllMaps();
	
	void lock();

	void unlock();

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	/*!***************************************************************************************
	\brief  Retrieves the type of component this instance is. SceneSystem requirement.
	\return componentType - The type of this component.
	*****************************************************************************************/
	componentType const type() const override { return inputComponent::getType(); }
	/*!***************************************************************************************
	\brief  Retrieves the type of component this class is.  SceneSystem requirement.
	\return componentType - The type of component this class represents.
	*****************************************************************************************/
	static componentType const getType() { static componentType const s_type = componentType::forConcrete<inputComponent>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Input", true, true, false); }

	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
private:

	std::mutex m_write_mutex;

	std::string context; //!< the context filter this component belongs to.
	std::unordered_map<unsigned int, std::string> scancodeToNameMap; //!< map of buttons to names
	std::unordered_map<std::string, bool> nameToStatusMap; //!< map of buttons names to status
	
	std::unordered_map<int, std::string> axisToNameMap; //!< map of axes to names
	std::unordered_map<std::string, float> nameToRangeMap; //!< map of axis names to status
	
	std::unordered_map<Cursor, std::string> cursorToNameMap; //!< map of cursors to names
	std::unordered_map<std::string, std::pair<double, double>> nameToPositionMap;//!< map of cursor names to status

	std::unordered_map<unsigned int, std::string> gamepadButtonToNameMap; //!< map of gamepad buttons to names
	std::unordered_map<std::string, actionType> gamepadNameToStatusMap; //!< map of gamepad butotn names to status
	std::unordered_map<std::string, unsigned int> gamepadButtonNameToControllerMap; //!< map of button names to controller id
	
	std::unordered_map<unsigned int, std::string> gamepadAxisToNameMap; //!< map of gamepad axes to names
	std::unordered_map<std::string, float> gamepadAxisNameToRangeMap; //!< map of gamepad axis names
	std::unordered_map<std::string, unsigned int> gamepadAxisNameToControllerMap; //!<map of axis names to controller id

	std::map<unsigned int, std::string> controllerToNameMap; //!< map of controllers to names
	std::unordered_map<std::string, actionType> controllerNameToStatusMap; //!< map of controller names to status




	struct request
	{
		enum class req_type { AddLetterToName, AddMouseButtonToName, AddAxisToName, AddCursorToName, 
			RemoveButton, RemoveAxis, RemoveCursor, AddGamepadButtonToName, AddGamepadAxisToName,
			RemoveGamepadButton, RemoveGamepadAxis, AddControllerState, RemoveControllerState } type;
		unsigned int letter;
		unsigned int mouseButton;
		unsigned int axisCode;
		unsigned int gamepadCode;
		unsigned int gamepadAxisCode;
		unsigned int gamepadID;
		Cursor cursorCode;
		std::string name;

		request(req_type p_type, unsigned int p_letter, unsigned int p_mouseButton, unsigned int p_axisCode, Cursor p_cursorCode, unsigned int p_gamepadButton, unsigned int p_gamepadAxis, unsigned int p_gamepadID, std::string p_name)
			: type{ p_type }, letter{ p_letter }, mouseButton{ p_mouseButton },
			axisCode{ p_axisCode }, gamepadCode{ p_gamepadButton }, gamepadAxisCode{ p_gamepadAxis }, gamepadID{ p_gamepadID }, cursorCode{ p_cursorCode }, name{ p_name }
		{ }
		static request makeAddLetterToName(unsigned int p_letter, std::string p_name)
		{
			return request(req_type::AddLetterToName, p_letter, 0, 0, Cursor::Default_Mouse, 0,0, 0, p_name);
		}
		static request makeAddMouseButtonToName(unsigned int p_mouseButton, std::string p_name)
		{
			return request(req_type::AddMouseButtonToName, 0, p_mouseButton, 0, Cursor::Default_Mouse, 0, 0, 0, p_name);
		}
		static request makeAddAxisToName(int p_axiscode, std::string p_name)
		{
			return request(req_type::AddAxisToName, 0, 0, p_axiscode, Cursor::Default_Mouse, 0, 0, 0, p_name);
		}
		static request makeAddCursorToName(Cursor p_cursorcode, std::string p_name)
		{
			return request(req_type::AddCursorToName, 0, 0, 0, p_cursorcode, 0, 0, 0, p_name);
		}
		static request makeRemoveButton(std::string p_name)
		{
			return request(req_type::RemoveButton, 0, 0, 0, Cursor::Default_Mouse, 0, 0, 0, p_name);
		}
		static request makeRemoveAxis(std::string p_name)
		{
			return request(req_type::RemoveAxis, 0, 0, 0, Cursor::Default_Mouse, 0, 0, 0, p_name);
		}
		static request makeRemoveCursor(std::string p_name)
		{
			return request(req_type::RemoveCursor, 0, 0, 0, Cursor::Default_Mouse, 0, 0, 0, p_name);
		}
		static request makeAddGamepadButton(unsigned int p_gamepadButton, unsigned int p_id, std::string p_name)
		{
			return request(req_type::AddGamepadButtonToName, 0, 0, 0, Cursor::Default_Mouse, p_gamepadButton, 0, p_id, p_name);
		}
		static request makeAddGamepadAxis(unsigned int p_gamepadAxis, unsigned int p_id, std::string p_name)
		{
			return request(req_type::AddGamepadAxisToName, 0, 0, 0, Cursor::Default_Mouse, 0, p_gamepadAxis, p_id, p_name);
		}

		static request makeAddControllerState(unsigned int p_id, std::string p_name)
		{
			return request(req_type::AddControllerState, 0, 0, 0, Cursor::Default_Mouse, 0, 0, p_id, p_name);
		}

		static request makeRemoveGamepadButton(std::string p_name)
		{
			return request(req_type::RemoveGamepadButton, 0, 0, 0, Cursor::Default_Mouse, 0, 0, 0, p_name);
		}

		static request makeRemoveGamepadAxis(std::string p_name)
		{
			return request(req_type::RemoveGamepadAxis, 0, 0, 0, Cursor::Default_Mouse, 0, 0, 0, p_name);
		}

		static request makeRemoveControllerState(std::string p_name)
		{
			return request(req_type::RemoveControllerState, 0, 0, 0, Cursor::Default_Mouse, 0, 0, 0, p_name);
		}

	};

	/*!***************************************************************************************
	\brief Process request immediately instead of queue it
	\param request Request to process
	*****************************************************************************************/
	void processRequest(const request& request);

	std::queue<inputComponent::request> m_processing_requests;
	
	/*!***************************************************************************************
	\brief  Allows the input system to update the values efficiently.
	\relates inputSystem
	*****************************************************************************************/
	friend class inputSystem;

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

};

