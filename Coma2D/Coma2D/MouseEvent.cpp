/*
* filename	MouseEvent.cpp
* fileinfo	마우스 이벤트 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "MouseEvent.h"

COMA_USING_NS

const std::string MouseEvent::MIDDLE_CLICK = "mouse_middleClick";
const std::string MouseEvent::MIDDLE_MOUSE_DOWN = "mouse_middleMouseDown";
const std::string MouseEvent::MIDDLE_MOUSE_UP = "mouse_middleMouseUp";
const std::string MouseEvent::CLICK = "mouse_click";
const std::string MouseEvent::MOUSE_DOWN = "mouse_mouseDown";
const std::string MouseEvent::MOUSE_UP = "mouse_mouseUp";
const std::string MouseEvent::MOUSE_MOVE = "mouse_mouseMove";
const std::string MouseEvent::MOUSE_OUT = "mouse_mouseOut";
const std::string MouseEvent::MOUSE_OVER = "mouse_mouseOver";
const std::string MouseEvent::MOUSE_WHEEL = "mouse_mouseWheel";
const std::string MouseEvent::RIGHT_CLICK = "mouse_rightClick";
const std::string MouseEvent::RIGHT_MOUSE_DOWN = "mouse_rightMouseDown";
const std::string MouseEvent::RIGHT_MOUSE_UP = "mouse_rightMouseUp";

MouseEvent::MouseEvent(const std::string& type, bool buttonDown, bool altKey, bool ctrlKey, bool shiftKey, int delta, float stageX, float stageY, float localX, float localY)
	:Event(type), buttonDown_(buttonDown), altKey_(altKey), ctrlKey_(ctrlKey), shiftKey_(shiftKey), delta_(delta), stageX_(stageX), stageY_(stageY), localX_(localX), localY_(localY)
{
}


MouseEvent::~MouseEvent()
{
}
