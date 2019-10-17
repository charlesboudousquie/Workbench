/*!***************************************************************************************
\file       EventTemplate.hpp
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once

typedef unsigned short EventID;

class EventBase
{
public:
	static EventID getNextTypeID() { static EventID numTypes; return numTypes++; }
	virtual EventID GetTypeID() const = 0;
};

template <typename EventType>
class EventTemplate : public EventBase
{
public:
	EventID GetTypeID() const override
	{
		return TypeID;
	}

	static const EventID TypeID;
};

template <typename T> const EventID EventTemplate<T>::TypeID = EventBase::getNextTypeID();
