/*!***************************************************************************************
\file       SystemBase.cpp
\author     Aaron Damyen
\date       5/7/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief  Implementation of the system class public members.
*****************************************************************************************/
//======== Self Include ================================================================//
#include "SystemBase.hpp"
//======== 1st Party Includes ==========================================================//
#include "SystemManagement/SystemManagerInterface.hpp"
#include "Events/EventSystem.hpp"
#include "SceneManagement/SceneSystem.hpp"
//======== 3rd Party Includes ==========================================================//
#include <Logger.hpp>
#include <typeindex>
#include <thread>

#include <EventTemplate.hpp>

//======== Types =======================================================================//
class preShutdownEvent : public EventTemplate<preShutdownEvent>
{};

//======== Defines =====================================================================//
//======== Static Declarations =========================================================//

//======== Constructors & Destructor ===================================================//

//======== Getters & Setters ===========================================================//

//======== Overrides ===================================================================//

//======== Functionality ===============================================================//

void systemBase::initialize(systemManagerInterface *p_system_manager)
{
	if (m_state != systemState::enm_Uninitialized)
		logger("systemBase").error() << "Initializing a system that is already initialized: " << this->name();
	m_system_manager = p_system_manager;
	setupEvents();
	onInitialize();
	m_state = systemState::enm_Running;
	updateDT(std::chrono::high_resolution_clock::now(), 0.0f);  // pump dt to clear out old initial value
}

void systemBase::startFrame()
{
	if(m_should_run_this_call)
	{
		onStartFrame();
	}
}

void systemBase::update()
{
	if (m_should_run_this_call)
	{
		onUpdate();
	}
}

void systemBase::endFrame()
{
	if(m_should_run_this_call)
	{
		onEndFrame();
	}
}

void systemBase::shutdown()
{
	if(m_state != systemState::enm_Running)
		logger("SystemBase").error() << "Shutting down " << this->name() << " on thread " << std::this_thread::get_id();
	teardownEvents();
	onShutdown();
	m_state = systemState::enm_Uninitialized;
	//logger("SystemBase").warn("System Shutdown!!!");
}

void systemBase::handleMessage(message& p_message)
{
	onHandleMessage(p_message);
}

systemBase::systemState systemBase::getState()
{
  return m_state;
}


void systemBase::onShutdown()
{

}

systemManagerInterface* systemBase::getSystemManager() const
{
	return m_system_manager;
}

void systemBase::updateDT(const HRC_time_point_t & p_current_time, float p_core_dt)
{
	// calculate and set dt
	m_time_to_next_frame -= p_core_dt;
	if(m_time_to_next_frame <= 0.0f)
	{
		m_should_run_this_call = true;
		m_dt = (p_current_time - m_lastTime).count() / float(1E09);
		m_lastTime = p_current_time; // update last frame start
		m_time_to_next_frame += m_desired_frame_time;
	}
	else
	{
		m_should_run_this_call = false;
	}
}

float systemBase::getDt() const
{
	return m_dt;
}

void systemBase::setDesiredFrameTime(float p_frame_time)
{
	m_desired_frame_time = p_frame_time;
}

void systemBase::setupEvents()
{
	auto l_event_sys = getSystemManager()->getSystem<eventSystem>();
	if (l_event_sys)
	{
		l_event_sys->RegisterEventCallback<systemBase, levelLoadEvent, &systemBase::handleLoadLevelEvent>(this);
		l_event_sys->RegisterEventCallback<systemBase, levelUnloadEvent, &systemBase::handleUnloadLevelEvent>(this);
		l_event_sys->RegisterEventCallback<systemBase, preShutdownEvent, &systemBase::handlePreShutdownEvent>(this);
	}
}

void systemBase::teardownEvents()
{
}

void systemBase::handleLoadLevelEvent(levelLoadEvent* p_event)
{
	onLevelLoad(p_event);
}

void systemBase::handleUnloadLevelEvent(levelUnloadEvent* p_event)
{
	onLevelUnload(p_event);
}

void systemBase::handlePreShutdownEvent(preShutdownEvent* /*p_event*/)
{
	onPreShutdown();
}

//======== Helper Functions ============================================================//
