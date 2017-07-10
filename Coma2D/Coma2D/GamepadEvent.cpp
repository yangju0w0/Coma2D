/*
* filename	GamepadEvent.cpp
* fileinfo	게임패드 이벤트 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "GamepadEvent.h"

COMA_USING_NS

const std::string GamepadEvent::BUTTON_DOWN		= "gamepad_down";
const std::string GamepadEvent::BUTTON_CLICK	= "gamepad_click";
const std::string GamepadEvent::BUTTON_UP		= "gamepad_up";
const std::string GamepadEvent::TRIGGER_INPUT	= "gamepad_trigger";
const std::string GamepadEvent::THUMB_INPUT		= "gamepad_thumb";
const std::string GamepadEvent::CONNECTED		= "gamepad_connected";
const std::string GamepadEvent::DISCONNECTED	= "gamepad_disconnected";

GamepadEvent::GamepadEvent(std::string type, Gamepad* target, DWORD buttonCode, bool left, bool right)
	:Event(type), target(target), buttonCode(buttonCode), left(left), right(right)
{
	if (left)
	{
		trigger = target->getLeftTrigger();
		thumbX = target->getLeftThumbX();
		thumbY = target->getLeftThumbY();
	}
	else if (right)
	{
		trigger = target->getRightTrigger();
		thumbX = target->getRightThumbX();
		thumbY = target->getRightThumbY();
	}
}


GamepadEvent::~GamepadEvent()
{
}
