/*
* filename	MouseEvent.h
* fileinfo	마우스 이벤트 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#pragma once
#include "Event.h"

COMA_NS_BEGIN

class MouseEvent :
	public Event
{
public:
	MouseEvent(const std::string& type, bool buttonDown = false, bool altKey = false, bool ctrlKey = false, bool shiftKey = false, int delta = 0, float stageX = 0, float stageY = 0, float localX = 0, float localY = 0);
	virtual ~MouseEvent();

public:
	static const std::string MIDDLE_CLICK;
	static const std::string MIDDLE_MOUSE_DOWN;
	static const std::string MIDDLE_MOUSE_UP;

	static const std::string CLICK;
	static const std::string MOUSE_DOWN;
	static const std::string MOUSE_UP;
	static const std::string MOUSE_MOVE;
	static const std::string MOUSE_OUT;
	static const std::string MOUSE_OVER;
	static const std::string MOUSE_WHEEL;

	static const std::string RIGHT_CLICK;
	static const std::string RIGHT_MOUSE_DOWN;
	static const std::string RIGHT_MOUSE_UP;

public:
	bool GetButtonDown() const { return buttonDown_; }
	bool GetAltKey() const { return altKey_; }
	bool GetCtrlKey() const { return ctrlKey_; }
	bool GetShiftKey() const { return shiftKey_; }

	int GetDelta() const { return delta_; }

	float GetStageX() const { return stageX_; }
	float GetStageY() const { return stageY_; }
	float GetLocalX() const { return localX_; }
	float GetLocalY() const { return localY_; }

private:
	bool buttonDown_;
	bool altKey_;
	bool ctrlKey_;
	bool shiftKey_;

	int delta_;

	float stageX_;
	float stageY_;

	float localX_;
	float localY_;
};

COMA_END