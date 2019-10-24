/*!***************************************************************************************
\file       EventSystem.cpp
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "EventSystem.hpp"

EventInternals::EventManager::EventManager()
{
}

void EventInternals::EventManager::QueueEvent(EventBase* incomingEvent)
{
	if (incomingEvent)
	{
		EventQueue.push(incomingEvent);
	}
}

void EventInternals::EventManager::ParseAllEvents()
{
	while (!EventQueue.empty())
	{
		EventBase* topEvent = EventQueue.front();
		EventQueue.pop();

		EventID EventType = topEvent->GetTypeID();

    //Event Type - 1 because of array indexing starting at 0 not 1
		std::vector<EventCallback*>& eventListeners = Listeners[EventType];

		for (size_t i = 0, size = eventListeners.size(); i < size; ++i)
		{
			EventCallback* callback = eventListeners[i];
			(*callback)(topEvent);
		}

		delete topEvent;
	}
}

