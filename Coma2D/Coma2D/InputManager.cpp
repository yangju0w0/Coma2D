/*
* filename	InputManager.cpp
* fileinfo	사용자 입력 관리 클래스 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "InputManager.h"
#include "MouseEvent.h"
#include "KeyboardEvent.h"
#include <hidusage.h>

COMA_USING_NS

InputManager::InputManager()
	:charIn(0), inputString(""), mousePosition(Point{0, 0}), leftMouseDowned(false), rightMouseDowned(false), middleMouseDowned(false)
{
	for (int i = 0; i < 256; i++)
	{
		pressedKey[i] = false;
	}
	for (int i = 0; i < MAX_CONTROLLERS; i++)
	{
		gamePads[i] = new Gamepad(i);
		gamePads[i]->updateConntected();
	}
}


InputManager::~InputManager()
{
	for (int i = 0; i < MAX_CONTROLLERS; i++)
	{
		delete gamePads[i];
	}
}

bool InputManager::createInputEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (wParam < 256)
		{
			pressedKey[wParam] = true;
		}
		dispatchEvent(new KeyboardEvent(KeyboardEvent::KEY_DOWN, 0, wParam, isKeyDown(VK_MENU), isKeyDown(VK_CONTROL), isKeyDown(VK_SHIFT)));
		return true;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		if (wParam < 256)
		{
			pressedKey[wParam] = false;
		}
		dispatchEvent(new KeyboardEvent(KeyboardEvent::KEY_UP, 0, wParam, isKeyDown(VK_MENU), isKeyDown(VK_CONTROL), isKeyDown(VK_SHIFT)));
		return true;
	case WM_CHAR:
		dispatchEvent(new KeyboardEvent(KeyboardEvent::KEY_CHAR, wParam, 0, isKeyDown(VK_MENU), isKeyDown(VK_CONTROL), isKeyDown(VK_SHIFT)));
		return true;
	case WM_MOUSEMOVE:
		updateMousePosition(lParam);
		dispatchEvent(new MouseEvent(MouseEvent::MOUSE_MOVE, leftMouseDowned, isKeyDown(VK_MENU), isKeyDown(VK_CONTROL), isKeyDown(VK_SHIFT), 0, mousePosition.x, mousePosition.y, mousePosition.x, mousePosition.y));
		return true;
	case WM_LBUTTONDOWN:
		leftMouseDowned = true;
		updateMousePosition(lParam);
		dispatchEvent(new MouseEvent(MouseEvent::MOUSE_DOWN, leftMouseDowned, isKeyDown(VK_MENU), isKeyDown(VK_CONTROL), isKeyDown(VK_SHIFT), 0, mousePosition.x, mousePosition.y, mousePosition.x, mousePosition.y));
		dispatchEvent(new MouseEvent(MouseEvent::CLICK, leftMouseDowned, isKeyDown(VK_MENU), isKeyDown(VK_CONTROL), isKeyDown(VK_SHIFT), 0, mousePosition.x, mousePosition.y, mousePosition.x, mousePosition.y));
		return true;
	case WM_LBUTTONUP:
		leftMouseDowned = false;
		updateMousePosition(lParam);
		dispatchEvent(new MouseEvent(MouseEvent::MOUSE_MOVE, leftMouseDowned, isKeyDown(VK_MENU), isKeyDown(VK_CONTROL), isKeyDown(VK_SHIFT), 0, mousePosition.x, mousePosition.y, mousePosition.x, mousePosition.y));
		return true;
	case WM_MBUTTONDOWN:
		middleMouseDowned = true;
		updateMousePosition(lParam);
		dispatchEvent(new MouseEvent(MouseEvent::MIDDLE_MOUSE_DOWN, middleMouseDowned, isKeyDown(VK_MENU), isKeyDown(VK_CONTROL), isKeyDown(VK_SHIFT), 0, mousePosition.x, mousePosition.y, mousePosition.x, mousePosition.y));
		dispatchEvent(new MouseEvent(MouseEvent::MIDDLE_CLICK, middleMouseDowned, isKeyDown(VK_MENU), isKeyDown(VK_CONTROL), isKeyDown(VK_SHIFT), 0, mousePosition.x, mousePosition.y, mousePosition.x, mousePosition.y));
		return true;
	case WM_MBUTTONUP:
		middleMouseDowned = false;
		updateMousePosition(lParam);
		dispatchEvent(new MouseEvent(MouseEvent::MIDDLE_MOUSE_UP, middleMouseDowned, isKeyDown(VK_MENU), isKeyDown(VK_CONTROL), isKeyDown(VK_SHIFT), 0, mousePosition.x, mousePosition.y, mousePosition.x, mousePosition.y));
	case WM_RBUTTONDOWN:
		rightMouseDowned = true;
		updateMousePosition(lParam);
		dispatchEvent(new MouseEvent(MouseEvent::RIGHT_MOUSE_DOWN, rightMouseDowned, isKeyDown(VK_MENU), isKeyDown(VK_CONTROL), isKeyDown(VK_SHIFT), 0, mousePosition.x, mousePosition.y, mousePosition.x, mousePosition.y));
		dispatchEvent(new MouseEvent(MouseEvent::RIGHT_CLICK, rightMouseDowned, isKeyDown(VK_MENU), isKeyDown(VK_CONTROL), isKeyDown(VK_SHIFT), 0, mousePosition.x, mousePosition.y, mousePosition.x, mousePosition.y));
		return true;
	case WM_RBUTTONUP:
		rightMouseDowned = false;
		updateMousePosition(lParam);
		dispatchEvent(new MouseEvent(MouseEvent::RIGHT_MOUSE_UP, rightMouseDowned, isKeyDown(VK_MENU), isKeyDown(VK_CONTROL), isKeyDown(VK_SHIFT), 0, mousePosition.x, mousePosition.y, mousePosition.x, mousePosition.y));
		return true;
	case WM_MOUSEWHEEL:
		dispatchEvent(new MouseEvent(MouseEvent::MOUSE_WHEEL, leftMouseDowned, isKeyDown(VK_MENU), isKeyDown(VK_CONTROL), isKeyDown(VK_SHIFT), (SHORT)HIWORD(wParam), mousePosition.x, mousePosition.y, mousePosition.x, mousePosition.y));
		return true;
	case WM_DEVICECHANGE:
		updateGamepadConnected();
		return true;
	}
	return false;
}

void InputManager::updateMousePosition(LPARAM lParam)
{
	mousePosition.x = LOWORD(lParam);
	mousePosition.y = HIWORD(lParam);
}
void InputManager::updateGamepadConnected()
{
	for (int i = 0; i < MAX_CONTROLLERS; i++)
	{
		gamePads[i]->updateConntected();
	}
}
void InputManager::updateGamepad(double deltaTime)
{
	for (int i = 0; i < MAX_CONTROLLERS; i++)
	{
		gamePads[i]->update(deltaTime);
	}
}