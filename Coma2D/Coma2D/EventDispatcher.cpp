/*
* filename	EventDispatcher.cpp
* fileinfo	이벤트 분배 및 처리 클래스 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "EventDispatcher.h"
COMA_USING_NS

EventDispatcher::EventDispatcher()
{
}


EventDispatcher::~EventDispatcher()
{
	listenerList.clear();
}

void EventDispatcher::setEventListener(const std::string& type, EventFunction function, void* target)
{
	for (unsigned int i = 0; i < listenerList.size(); i++)
	{
		if (listenerList[i].type == type)
		{
			if (&listenerList[i].function == &function)
				return;
		}
	}
	listenerList.push_back(Listener{ type, function, target });
}

void EventDispatcher::removeEventListener(const std::string& type, void* target)
{
	for (unsigned int i = 0; i < listenerList.size(); i++)
	{
		if (listenerList[i].type == type)
		{
			if (listenerList[i].target == target)
			{
				listenerList.erase(listenerList.begin() + i);
				i--;
			}
		}
	}
}
bool EventDispatcher::hasEventListener(const std::string& type)
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
			if (listenerList[i].target)
				listenerList[i].function(event);
			else
				listenerList.erase(listenerList.begin() + i);
		}
	}
	delete event;
}