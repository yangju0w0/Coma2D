/*
* filename	InputManager.h
* fileinfo	사용자 입력 관리 클래스 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#pragma once
#include "Manager.h"
#include "Gamepad.h"

COMA_NS_BEGIN

class InputManager :
	public Manager
{
public:
	InputManager();
	virtual ~InputManager();

public:
	bool CreateInputEvent(UINT uMsg, WPARAM wParam, LPARAM lParam); //이벤트가 처리되지 않을 경우, false를 반환한다.
	bool IsKeyDown(unsigned int keyCode) const { return pressedKey_[keyCode]; }
	bool IsLeftMouseDowned() const { return leftMouseDowned_; }
	bool IsRightMouseDowned() const { return rightMouseDowned_; }
	bool IsMiddleMouseDowned() const { return middleMouseDowned_; }
	Point GetMousePosition() const { return mousePosition_; }

	void UpdateGamepad(double deltaTime);
	Gamepad* GetGamepad(UINT index) const;

private:
	bool pressedKey_[256];
	char charIn_;
	std::string inputString_;

	Point mousePosition_;
	bool leftMouseDowned_;
	bool rightMouseDowned_;
	bool middleMouseDowned_;

	Gamepad* gamePads_[MAX_CONTROLLERS];

	void UpdateMousePosition(LPARAM lParam);
	void UpdateGamepadConnected();
};

COMA_END