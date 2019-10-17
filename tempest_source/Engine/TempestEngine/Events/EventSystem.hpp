/*!***************************************************************************************
\file       EventSystem.hpp
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once

#include "../SystemBase.hpp"
#include <vector>
#include <queue>
#include <unordered_map>

#include <EventTemplate.hpp>


	class EventCallback
	{
	public:
		template <typename classType>
		EventCallback(classType* instance, EventID TypeID) : EventTypeID(TypeID), classInstance(instance)
		{
		}

		EventID GetEventTypeID() { return EventTypeID; }

		virtual void operator()(EventBase* InputEvent) = 0;

	private:
		EventID EventTypeID;
		void* classInstance;
	};

	template <typename EventType, typename ClassType>
	class EventCallbackTemplate : public EventCallback
	{
	public:

		EventCallbackTemplate(EventID TypeID, ClassType* ClassInstance, void(ClassType::*MemberFunc)(EventType*))
			: EventCallback(ClassInstance, TypeID), ClassInstance(ClassInstance), MemberFunc(MemberFunc)
		{
			// Intentionally left blank
		}

		EventID GetEventID() { return GetEventTypeID(); }

		void operator()(EventBase* InputEvent) override
		{
			EventType* OutputEvent = reinterpret_cast<EventType*>(InputEvent);
			(ClassInstance->*MemberFunc)(OutputEvent);
		}

	private:

		ClassType* ClassInstance;
		void(ClassType::*MemberFunc)(EventType*);
	};

	namespace EventInternals
	{
		class EventManager
		{
		public:

			EventManager();

			void ParseAllEvents();

			void StartParsing() { runningFlag = true; }
			void StopParsing() { runningFlag = false; }

			template <typename ClassType, typename EventType, void (ClassType::*Function)(EventType*)>
			void RegisterEventCallback(ClassType* ObjectInstance);

			void QueueEvent(EventBase* incomingEvent);

		private:

			bool runningFlag;
			std::queue<EventBase*> EventQueue;
			//std::vector<EventCallback*> Listeners;
			std::unordered_map<EventID, std::vector<EventCallback*>> Listeners;
		};

		// Note: if there was a previous event callback for a specific event type this will overwrite the old listener
		template <typename ClassType, typename EventType, void (ClassType::*Function)(EventType*)>
		void EventManager::RegisterEventCallback(ClassType * ObjectInstance)
		{
			EventCallback* newCallback = new EventCallbackTemplate<EventType, ClassType>(EventType::TypeID, ObjectInstance, Function);

			EventID CallbackType = newCallback->GetEventTypeID();

			Listeners[CallbackType].push_back(newCallback);
		}
	}


class eventSystem final :public systemBase
{
public:
	eventSystem() = default;
	static const std::string& getName() { static const std::string n("eventSystem"); return n; }
	const std::string& name() const override { return getName(); };
	void onUpdate() override { m_EventManager.ParseAllEvents(); }

	template <typename ClassType, typename EventType, void (ClassType::*Function)(EventType*)>
	void RegisterEventCallback(ClassType * ObjectInstance)
	{
		m_EventManager.RegisterEventCallback<ClassType, EventType, Function>(ObjectInstance);
	}

	void QueueEvent(EventBase* incomingEvent) { m_EventManager.QueueEvent(incomingEvent); }

private:
	EventInternals::EventManager m_EventManager;
};
