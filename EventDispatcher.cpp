#include "stdafx.h"
#include "EventDispatcher.h"
using namespace coma2d;

EventDispatcher::EventDispatcher()
{
}


EventDispatcher::~EventDispatcher()
{
	
}

void EventDispatcher::setEventListener(const char* type, EventFunction function)
{
	for (unsigned int i = 0; i < listenerList.size(); i++)
	{
		if (listenerList[i].type == type)
		{
			if (&listenerList[i].function == &function)
				return;
		}
	}
	listenerList.push_back(Listener{ type, function });
}

void EventDispatcher::removeEventListener(const char* type, EventFunction function)
{
	for (unsigned int i = 0; i < listenerList.size(); i++)
	{
		if (listenerList[i].type == type)
		{
			if (&listenerList[i].function == &function)
			{
				listenerList.erase(listenerList.begin() + i);
				return;
			}
		}
	}
}
bool EventDispatcher::hasEventListener(const char* type)
{
	for (unsigned int i = 0; i < listenerList.size(); i++)
	{
		if (listenerList[i].type == type)
			return true;
	}
	return false;
}


void EventDispatcher::dispatchEvent(Event* event)
{
	for (unsigned int i = 0; i < listenerList.size(); i++)
	{
		if (listenerList[i].type == event->getType())
		{
			if (listenerList[i].function)
				listenerList[i].function(event);
			else
				listenerList.erase(listenerList.begin() + i);
		}
	}
}