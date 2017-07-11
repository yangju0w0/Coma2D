/*
* filename	EventDispatcher.cpp
* fileinfo	이벤트 분배 및 처리 클래스 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "EventDispatcher.h"
COMA_USING_NS

EventDispatcher::EventDispatcher()
{}

EventDispatcher::~EventDispatcher()
{
	listenerList_.clear();
}

void EventDispatcher::SetEventListener(const std::string& type, EventCallback function, void* target)
{
	for (const auto& iter : listenerList_)
	{
		if (iter.type == type && &iter.function == &function)
		{
			return;
		}
	}
	listenerList_.push_back(Listener{ type, function, target });
}

void EventDispatcher::RemoveEventListener(const std::string& type, void* target)
{
	for (auto iter = listenerList_.begin(); iter != listenerList_.end(); )
	{
		if ((*iter).type == type && (*iter).target == target)
		{
			iter = listenerList_.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

bool EventDispatcher::HasEventListener(const std::string& type)
{
	for (const auto& iter : listenerList_)
	{
		if (iter.type == type)
		{
			return true;
		}
	}
	return false;
}

void EventDispatcher::DispatchEvent(Event* event)
{
	for (const auto& iter : listenerList_)
	{
		if (iter.type == event->GetType())
		{
			if (iter.target)
			{
				iter.function(event);
			}
		}
	}
	delete event;
}