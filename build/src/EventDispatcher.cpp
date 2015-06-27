#include "EventDispatcher.h"
COMA_USING_NS

EventDispatcher::EventDispatcher()
{
}


EventDispatcher::~EventDispatcher()
{
	listenerList.clear();
}

void EventDispatcher::setEventListener(std::string type, EventFunction function)
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

void EventDispatcher::removeEventListener(std::string type, EventFunction function)
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
bool EventDispatcher::hasEventListener(std::string type)
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
	delete event;
}