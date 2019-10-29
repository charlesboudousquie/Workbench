/*!***************************************************************************************
\file       SystemBase.hpp
\author     Aaron Damyen
\date       5/7/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief  Definition of the system public interface.  This is the base for all back-end systems
within the engine.
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "SystemManagement/SystemManagerInterface.hpp"
//======== 3rd Party Includes ==========================================================//
#include <chrono>
#include <string>
#include "EventHandler.hpp"
//#include <EventTemplate.hpp>
//======== Types =======================================================================//

using HRC_time_point_t = decltype(std::chrono::high_resolution_clock::now());

//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//
struct message;
class systemManager;
class levelLoadEvent;
class levelUnloadEvent;
class preShutdownEvent;


/*!*************************************************************************************
\par class: systemBase
\brief   This is the base class for all engine systems.  Provides a common interface into the system.
***************************************************************************************/
class systemBase: public EventSystem3::EventHandler
{
	public:
		systemBase();

		/*!*******************************************************************************
		\brief  Retrieves the type of component this instance is. SceneSystem requirement.
		\return componentType - The type of this component.
		*********************************************************************************/
		virtual const std::string& name() const = 0;

		/*!***************************************************************************************
		\par enum: systemState
		\brief Records the current state of this system
		*****************************************************************************************/
		enum class systemState
		{
			enm_Uninitialized,
			enm_Running,
		};

		/*!***************************************************************************************
		\brief  Alows inherited clases te destory.
		*****************************************************************************************/
		virtual ~systemBase() = default;

		/*!***************************************************************************************
		\brief  Starts the initialization process for the derived system.
		\param p_system_manager - that owns this system
		*****************************************************************************************/
		void initialize(systemManagerInterface * p_system_manager);

		/*!***************************************************************************************
		\brief  Starts the frame for the derived system.
		*****************************************************************************************/
		void startFrame();

		/*!***************************************************************************************
		\brief  Starts the update process for the derived system.
		*****************************************************************************************/
		void update();

		/*!***************************************************************************************
		\brief  Ends the frame for the derived system.
		*****************************************************************************************/
		void endFrame();

		/*!***************************************************************************************
		\brief  Starts the shutdown process for the derived system.
		*****************************************************************************************/
		void shutdown();

		/*!***************************************************************************************
		\brief  Notifies the system on an incoming message
		*****************************************************************************************/
		void handleMessage(message & p_message);

    systemState getState();

		/*!***************************************************************************************
		\brief  Gets the delta time for this system as a float.
		\return Delta Time in seconds.
		*****************************************************************************************/
		float getDt() const;

		/*!***************************************************************************************
		\brief  Updates the delta time for the frame
		\param p_current_time - The current execution time of this frame
		\param p_core_dt - the amount of time changed from last call
		*****************************************************************************************/
		void updateDT(const HRC_time_point_t & p_current_time, float p_core_dt);

		void handleLoadLevelEvent(levelLoadEvent & p_event);
		void handleUnloadLevelEvent(levelUnloadEvent & p_event);
		void handlePreShutdownEvent(preShutdownEvent & p_event);
	protected:
		/*!***************************************************************************************
		\brief  Allows a system to specify it's own frame rate (default is 1/60 seconds)
		\param p_frame_time - the new desired frame time for this system.
		*****************************************************************************************/
		void setDesiredFrameTime(float p_frame_time);

		/*!***************************************************************************************
		\brief  Empty handling of the initialize message for systems that don't need to respond to it.
		*****************************************************************************************/
		virtual void onInitialize() {}

		/*!***************************************************************************************
		\brief  Empty handling of the level loading message for systems that don't need to respond to it.
		*****************************************************************************************/
		virtual void onLevelLoad(const levelLoadEvent & /*p_event*/) {};

		/*!***************************************************************************************
		\brief  Empty handling of the start frame message for systems that don't need to respond to it.
		*****************************************************************************************/
		virtual void onStartFrame() {};

		/*!***************************************************************************************
		\brief  Empty handling of the update message for systems that don't need to respond to it.
		*****************************************************************************************/
		virtual void onUpdate() {};

		/*!***************************************************************************************
		\brief  Empty handling of the end frame message for systems that don't need to respond to it.
		*****************************************************************************************/
		virtual void onEndFrame() {};

		/*!***************************************************************************************
		\brief  Empty handling of the level unloading message for systems that don't need to respond to it.
		*****************************************************************************************/
		virtual void onLevelUnload(const levelUnloadEvent & /*p_event*/) {};

		/*!***************************************************************************************
		\brief  Empty handling of the pre-shutdown message for systems that don't need to respond to it.
		*****************************************************************************************/
		virtual void onPreShutdown() {};

		/*!***************************************************************************************
		\brief  Empty handling of the shutdown message for systems that don't need to respond to
		        it.
		*****************************************************************************************/
		virtual void onShutdown();

		/*!***************************************************************************************
		\brief  Empty handling of the incoming message notification for systems that don't
		        need to respond to it.
		*****************************************************************************************/
		virtual void onHandleMessage(message & /*p_message*/) {}

		/*!***************************************************************************************
		\brief  Gets the engine.
		\return Gets systemManager that owns this.
		*****************************************************************************************/
		systemManagerInterface* getSystemManager() const;


	private:

		// void setupEvents();
		// void teardownEvents();

		systemManagerInterface* m_system_manager = nullptr; //!< System manager that owns this

		std::chrono::high_resolution_clock::time_point m_lastTime
		= std::chrono::high_resolution_clock::now(); //!< Last frames start
		float m_dt = 0.0f; //!< current stored delta time in seconds
		float m_desired_frame_time = 1.0f / 30.0f; //!< desired frame time for this system. 1/30 seconds default.
		bool m_should_run_this_call = true; //!< flag to indicate if the derived methods should be called this value of dt
		float m_time_to_next_frame = 0.0f; //!< amount of time remaining until next frame
		systemState m_state = systemState::enm_Uninitialized; //!< The current state of this system
};
