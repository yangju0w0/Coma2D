/*
* filename	GamepadEvent.h
* fileinfo	게임패드 이벤트 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#pragma once
#include "Event.h"
#include "Gamepad.h"

COMA_NS_BEGIN

class GamepadEvent :
	public Event
{
public:
	GamepadEvent(std::string type, Gamepad* target, DWORD buttonCode = 0, bool left = false, bool right = false);
	virtual ~GamepadEvent();

public:
	static const std::string BUTTON_DOWN;
	static const std::string BUTTON_CLICK;
	static const std::string BUTTON_UP;
	static const std::string TRIGGER_INPUT;
	static const std::string THUMB_INPUT;
	static const std::string CONNECTED;
	static const std::string DISCONNECTED;

public:
	DWORD getButtonCode(){ return buttonCode; }
	float getTriggerValue(){ return trigger; }
	float getThumbX(){ return thumbX; }
	float getThumbY(){ return thumbY; }
	Point getThumbValue(){ return Point{ thumbX, thumbY }; }
	bool isLeft(){ return left; }
	bool isRight(){ return right; }
	Gamepad* getTarget(){ return target; }

private:
	DWORD buttonCode;
	float trigger;
	float thumbX;
	float thumbY;
	bool left;
	bool right;
	Gamepad* target;
};

COMA_END