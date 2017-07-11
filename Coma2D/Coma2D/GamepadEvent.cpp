/*
* filename	GamepadEvent.cpp
* fileinfo	게임패드 이벤트 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "GamepadEvent.h"

COMA_USING_NS

const std::string GamepadEvent::BUTTON_DOWN	= "gamepad_down";
const std::string GamepadEvent::BUTTON_CLICK = "gamepad_click";
const std::string GamepadEvent::BUTTON_UP = "gamepad_up";
const std::string GamepadEvent::TRIGGER_INPUT = "gamepad_trigger";
const std::string GamepadEvent::THUMB_INPUT = "gamepad_thumb";
const std::string GamepadEvent::CONNECTED = "gamepad_connected";
const std::string GamepadEvent::DISCONNECTED = "gamepad_disconnected";

GamepadEvent::GamepadEvent(const std::string& type, Gamepad* target, DWORD buttonCode, bool left, bool right)
	:Event(type), target_(target), buttonCode_(buttonCode), left_(left), right_(right)
{
	if (left)
	{
		trigger_ = target->getLeftTrigger();
		thumbX_ = target->getLeftThumbX();
		thumbY_ = target->getLeftThumbY();
	}
	else if (right)
	{
		trigger_ = target->getRightTrigger();
		thumbX_ = target->getRightThumbX();
		thumbY_ = target->getRightThumbY();
	}
}


GamepadEvent::~GamepadEvent()
{
}
