/*
* filename	KeyboardEvent.cpp
* fileinfo	키보드 이벤트 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "KeyboardEvent.h"

COMA_USING_NS

const std::string KeyboardEvent::KEY_DOWN	= "keyboard_keyDown";
const std::string KeyboardEvent::KEY_UP		= "keyboard_keyUp";
const std::string KeyboardEvent::KEY_CHAR	= "keyboard_keyChar";

KeyboardEvent::KeyboardEvent(std::string type, unsigned int charCode, unsigned int keyCode, bool altKey, bool ctrlKey, bool shiftKey)
	:Event(type), charCode(charCode), keyCode(keyCode), altKey(altKey), ctrlKey(ctrlKey), shiftKey(shiftKey)
{
}


KeyboardEvent::~KeyboardEvent()
{
}
