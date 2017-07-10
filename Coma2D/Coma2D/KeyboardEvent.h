/*
* filename	KeyboardEvent.h
* fileinfo	키보드 이벤트 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#pragma once
#include "Event.h"

COMA_NS_BEGIN

class KeyboardEvent :
	public Event
{
public:
	KeyboardEvent(std::string type, unsigned int charCode=0, unsigned int keyCode=0, bool altKey=false, bool ctrlKey=false, bool shiftKey=false);
	virtual ~KeyboardEvent();

public:
	static const std::string KEY_DOWN;
	static const std::string KEY_UP;
	static const std::string KEY_CHAR;

public:
	unsigned int getCharCode(){ return charCode; }
	unsigned int getKeyCode(){ return keyCode; }

	bool getAltKey(){ return altKey; }
	bool getCtrlKey(){ return ctrlKey; }
	bool getShiftKey(){ return shiftKey; }
private:
	unsigned int charCode;
	unsigned int keyCode;
	unsigned int keyLocation;

	bool altKey;
	bool ctrlKey;
	bool shiftKey;
};

COMA_END