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
	MouseEvent(std::string type, bool buttonDown=false,bool altKey=false, bool ctrlKey=false, bool shiftKey=false, int delta=0, float stageX=0, float stageY=0, float localX=0, float localY=0);
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
	bool	getButtonDown()	{ return buttonDown; }
	bool	getAltKey()		{ return altKey; }
	bool	getCtrlKey()	{ return ctrlKey; }
	bool	getShiftKey()	{ return shiftKey; }

	int		getDelta()		{ return delta; }
	
	float	getStageX()		{ return stageX; }
	float	getStageY()		{ return stageY; }
	float	getLocalX()		{ return localX; }
	float	getLocalY()		{ return localY; }

private:
	bool buttonDown;
	bool altKey;
	bool ctrlKey;
	bool shiftKey;

	int delta;

	float stageX;
	float stageY;

	float localX;
	float localY;
};

COMA_END