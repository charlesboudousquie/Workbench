/*!***************************************************************************************
\file       MessagingSystem.hpp
\author     Aaron Damyen
\date       7/12/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the system for routing and transfering messages
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "Message.hpp"
#include "MessageQueue.hpp"
#include "../SystemBase.hpp"

//========3rd Party Includes============================================================//

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: messagingSystem
\brief  This is the system for routing and transfering messages between systems.  Use
        messageQueue to send messages.
*****************************************************************************************/
class messagingSystem final : public systemBase
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:

	/*!***************************************************************************************
	\brief  retrieves the name of this system. There are two versions for the same functionality
			so that we can get the name from the class as well as an instance
	\return the name of the s
	*****************************************************************************************/
	void send(std::string p_sender, std::string p_destination, unsigned int p_message_code, void * p_data);
	void onHandleMessage(message& p_message) override;
	static const std::string& getName() { static const std::string n("messagingSystem"); return n; }
	virtual const std::string& name() const override { return getName(); }

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	////////========================================================================////////
	////////   Protected                                                            ////////
	////////========================================================================////////
protected:

	void onInitialize() override;
	void onUpdate() override;
	void onShutdown() override;

	std::mutex m_lock;
	messageQueue m_message_queue;

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

};



