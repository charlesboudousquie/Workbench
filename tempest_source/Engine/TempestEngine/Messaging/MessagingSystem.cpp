/*!***************************************************************************************
\file       MessagingSystem.cpp
\author     Aaron Damyen
\date       7/12/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the messagingSystem
*****************************************************************************************/

//========Self Include==================================================================//
#include "MessagingSystem.hpp"
#include "../SystemManager.hpp"
#include <Logger.hpp>

//========1st Party Includes============================================================//
//========3rd Party Includes============================================================//
//========Types=========================================================================//
//========Defines=======================================================================//
//========Static Deceleration===========================================================//

///////========================================================================///////
///////   Public                                                               ///////
///////========================================================================///////

//==============================================================================//
//        Constructor                                                           //
//==============================================================================//

//==============================================================================//
//        Destructor                                                            //
//==============================================================================//

//==============================================================================//
//        Getters & Setters                                                     //
//==============================================================================//

////==========================================================================////
////      Non-Virtual                                                         ////
////==========================================================================////

//////======================================================================//////
//////    Non-Static                                                        //////
//////======================================================================//////

/////========================================================================/////
/////     Functions                                                          /////
/////========================================================================/////

///////========================================================================///////
///////   Protected                                                            ///////
///////========================================================================///////

void messagingSystem::send(std::string p_sender, std::string p_destination, unsigned int p_message_code, void * p_data)
{
	m_message_queue.send(p_sender, p_destination, p_message_code, p_data);
}

void messagingSystem::onHandleMessage(message& p_message)
{
	logger mes_logger("messaging");
	mes_logger.debug(std::string{ "message received" });
	std::string messagecode = std::string("code is: ") + std::to_string(p_message.m_message_code);
	mes_logger.debug(messagecode);
}

void messagingSystem::onInitialize()
{
	m_message_queue.onInitialize();
}

static bool send_message_test = false;

void messagingSystem::onUpdate()
{
	if(!send_message_test)
	{
		//messagingSystem::send(SYSTEM_NAME, SYSTEM_NAME, 1, NULL);
		send_message_test = true;
	}
	//logger::error() << "entering onUpdate(): " << std::this_thread::get_id();
	std::lock_guard<std::mutex> l_lock(m_lock);
	//logger::error() << "  acquired lock: " << std::this_thread::get_id();
	while(!m_message_queue.empty())
	{
		//logger::error() << "in messagsystem update, non-empty" << std::endl;
		message l_msg = m_message_queue.pop();
		// find destination system
		systemBase * l_system = this->getSystemManager()->getSystem(l_msg.m_destination);
		// if we know the destination system, send the message on for processing
		if (l_system != nullptr)
		{
			// send message
			l_system->handleMessage(l_msg);
			// check if sender wanted notification of completion
			/*if(l_msg.m_expect_result && l_msg.m_completed_handler)
			{
				l_msg.m_completed_handler(l_msg, l_msg.m_result_data1);
			}*/
		}
	}
	//logger::error() << "exiting onUpdate()";
}

void messagingSystem::onShutdown()
{
	m_message_queue.onShutdown();
}

//////======================================================================//////
//////    Non-Static                                                        //////
//////======================================================================//////

/////========================================================================/////
/////     Functions                                                          /////
/////========================================================================/////

////==========================================================================////
////      Non-Virtual                                                         ////
////==========================================================================////

//==============================================================================//
//        Helper                                                                //
//==============================================================================//
