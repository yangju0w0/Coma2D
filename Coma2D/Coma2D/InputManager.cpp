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
	:charIn_(0), inputString_(""), mousePosition_(Point{ 0, 0 }), leftMouseDowned_(false), rightMouseDowned_(false), middleMouseDowned_(false)
{
	for (int i = 0; i < 256; i++)
	{
		pressedKey_[i] = false;
	}

	for (int i = 0; i < MAX_CONTROLLERS; i++)
	{
		gamePads_[i] = new Gamepad(i);
		gamePads_[i]->UpdateConntected();
	}
}

InputManager::~InputManager()
{
	for (int i = 0; i < MAX_CONTROLLERS; i++)
	{
		delete gamePads_[i];
	}
}

bool InputManager::CreateInputEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (wParam < 256)
		{
			pressedKey_[wParam] = true;
		}
		DispatchEvent(new KeyboardEvent(KeyboardEvent::KEY_DOWN, 0, wParam, IsKeyDown(VK_MENU), IsKeyDown(VK_CONTROL), IsKeyDown(VK_SHIFT)));
		return true;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		if (wParam < 256)
		{
			pressedKey_[wParam] = false;
		}
		DispatchEvent(new KeyboardEvent(KeyboardEvent::KEY_UP, 0, wParam, IsKeyDown(VK_MENU), IsKeyDown(VK_CONTROL), IsKeyDown(VK_SHIFT)));
		return true;

	case WM_CHAR:
		DispatchEvent(new KeyboardEvent(KeyboardEvent::KEY_CHAR, wParam, 0, IsKeyDown(VK_MENU), IsKeyDown(VK_CONTROL), IsKeyDown(VK_SHIFT)));
		return true;

	case WM_MOUSEMOVE:
		UpdateMousePosition(lParam);
		DispatchEvent(new MouseEvent(MouseEvent::MOUSE_MOVE, leftMouseDowned_, IsKeyDown(VK_MENU), IsKeyDown(VK_CONTROL), IsKeyDown(VK_SHIFT), 0, mousePosition_.x, mousePosition_.y, mousePosition_.x, mousePosition_.y));
		return true;

	case WM_LBUTTONDOWN:
		leftMouseDowned_ = true;
		UpdateMousePosition(lParam);
		DispatchEvent(new MouseEvent(MouseEvent::MOUSE_DOWN, leftMouseDowned_, IsKeyDown(VK_MENU), IsKeyDown(VK_CONTROL), IsKeyDown(VK_SHIFT), 0, mousePosition_.x, mousePosition_.y, mousePosition_.x, mousePosition_.y));
		DispatchEvent(new MouseEvent(MouseEvent::CLICK, leftMouseDowned_, IsKeyDown(VK_MENU), IsKeyDown(VK_CONTROL), IsKeyDown(VK_SHIFT), 0, mousePosition_.x, mousePosition_.y, mousePosition_.x, mousePosition_.y));
		return true;

	case WM_LBUTTONUP:
		leftMouseDowned_ = false;
		UpdateMousePosition(lParam);
		DispatchEvent(new MouseEvent(MouseEvent::MOUSE_MOVE, leftMouseDowned_, IsKeyDown(VK_MENU), IsKeyDown(VK_CONTROL), IsKeyDown(VK_SHIFT), 0, mousePosition_.x, mousePosition_.y, mousePosition_.x, mousePosition_.y));
		return true;

	case WM_MBUTTONDOWN:
		middleMouseDowned_ = true;
		UpdateMousePosition(lParam);
		DispatchEvent(new MouseEvent(MouseEvent::MIDDLE_MOUSE_DOWN, middleMouseDowned_, IsKeyDown(VK_MENU), IsKeyDown(VK_CONTROL), IsKeyDown(VK_SHIFT), 0, mousePosition_.x, mousePosition_.y, mousePosition_.x, mousePosition_.y));
		DispatchEvent(new MouseEvent(MouseEvent::MIDDLE_CLICK, middleMouseDowned_, IsKeyDown(VK_MENU), IsKeyDown(VK_CONTROL), IsKeyDown(VK_SHIFT), 0, mousePosition_.x, mousePosition_.y, mousePosition_.x, mousePosition_.y));
		return true;

	case WM_MBUTTONUP:
		middleMouseDowned_ = false;
		UpdateMousePosition(lParam);
		DispatchEvent(new MouseEvent(MouseEvent::MIDDLE_MOUSE_UP, middleMouseDowned_, IsKeyDown(VK_MENU), IsKeyDown(VK_CONTROL), IsKeyDown(VK_SHIFT), 0, mousePosition_.x, mousePosition_.y, mousePosition_.x, mousePosition_.y));

	case WM_RBUTTONDOWN:
		rightMouseDowned_ = true;
		UpdateMousePosition(lParam);
		DispatchEvent(new MouseEvent(MouseEvent::RIGHT_MOUSE_DOWN, rightMouseDowned_, IsKeyDown(VK_MENU), IsKeyDown(VK_CONTROL), IsKeyDown(VK_SHIFT), 0, mousePosition_.x, mousePosition_.y, mousePosition_.x, mousePosition_.y));
		DispatchEvent(new MouseEvent(MouseEvent::RIGHT_CLICK, rightMouseDowned_, IsKeyDown(VK_MENU), IsKeyDown(VK_CONTROL), IsKeyDown(VK_SHIFT), 0, mousePosition_.x, mousePosition_.y, mousePosition_.x, mousePosition_.y));
		return true;

	case WM_RBUTTONUP:
		rightMouseDowned_ = false;
		UpdateMousePosition(lParam);
		DispatchEvent(new MouseEvent(MouseEvent::RIGHT_MOUSE_UP, rightMouseDowned_, IsKeyDown(VK_MENU), IsKeyDown(VK_CONTROL), IsKeyDown(VK_SHIFT), 0, mousePosition_.x, mousePosition_.y, mousePosition_.x, mousePosition_.y));
		return true;

	case WM_MOUSEWHEEL:
		DispatchEvent(new MouseEvent(MouseEvent::MOUSE_WHEEL, leftMouseDowned_, IsKeyDown(VK_MENU), IsKeyDown(VK_CONTROL), IsKeyDown(VK_SHIFT), (SHORT)HIWORD(wParam), mousePosition_.x, mousePosition_.y, mousePosition_.x, mousePosition_.y));
		return true;

	case WM_DEVICECHANGE:
		UpdateGamepadConnected();
		return true;
	}
	return false;
}

Gamepad* InputManager::GetGamepad(UINT index) const
{
	if (index > MAX_CONTROLLERS - 1)
	{
		return gamePads_[MAX_CONTROLLERS - 1];
	}
	return gamePads_[index];
}

void InputManager::UpdateMousePosition(LPARAM lParam)
{
	mousePosition_.x = LOWORD(lParam);
	mousePosition_.y = HIWORD(lParam);
}

void InputManager::UpdateGamepadConnected()
{
	for (int i = 0; i < MAX_CONTROLLERS; ++i)
	{
		gamePads_[i]->UpdateConntected();
	}
}

void InputManager::UpdateGamepad(double deltaTime)
{
	for (int i = 0; i < MAX_CONTROLLERS; ++i)
	{
		gamePads_[i]->Update(deltaTime);
	}
}