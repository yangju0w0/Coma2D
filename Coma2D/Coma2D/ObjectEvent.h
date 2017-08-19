/*
* filename	ObjectEvent.h
* fileinfo	표시객체 이벤트 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#pragma once
#include "Event.h"
#include "DisplayObject.h"

COMA_NS_BEGIN

class ObjectEvent :
	public Event
{
public:
	ObjectEvent(const std::string& type, DisplayObject* target, double deltaTime = 0.0);
	virtual ~ObjectEvent();

public:
	static const std::string UPDATE;
	static const std::string RENDER;
	static const std::string ADDED;
	static const std::string REMOVED;

	double DeltaTime() const { return time_; }
	DisplayObject* GetTarget() const { return target_; }

private:
	double time_;
	DisplayObject* target_;
};

COMA_END