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
	GamepadEvent(const std::string& type, Gamepad* target, DWORD buttonCode = 0, bool left = false, bool right = false);
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
	DWORD GetButtonCode() const { return buttonCode_; }
	float GetTriggerValue() const { return trigger_; }
	float GetThumbX() const { return thumbX_; }
	float GetThumbY() const { return thumbY_; }
	Point GetThumbValue() const { return Point{ thumbX_, thumbY_ }; }
	bool IsLeft() const { return left_; }
	bool IsRight() const { return right_; }
	Gamepad* GetTarget() const { return target_; }

private:
	DWORD buttonCode_;
	float trigger_;
	float thumbX_;
	float thumbY_;
	bool left_;
	bool right_;
	Gamepad* target_;
};

COMA_END