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
	bool createInputEvent(UINT uMsg, WPARAM wParam, LPARAM lParam); //이벤트 처리가 발생하지 않을 경우, false를 반환한다.
	bool isKeyDown(unsigned int keyCode){ return pressedKey[keyCode]; }
	bool isLeftMouseDowned(){ return leftMouseDowned; }
	bool isRightMouseDowned(){ return rightMouseDowned; }
	bool isMiddleMouseDowned(){ return middleMouseDowned; }
	Point getMousePosition(){ return mousePosition; }

	void updateGamepad(double deltaTime);
	Gamepad* getGamepad(UINT index){ if (index > MAX_CONTROLLERS - 1) return gamePads[MAX_CONTROLLERS - 1]; return gamePads[index]; }

private:
	bool pressedKey[256];
	char charIn;
	std::string inputString;

	Point mousePosition;
	bool leftMouseDowned;
	bool rightMouseDowned;
	bool middleMouseDowned;

	Gamepad* gamePads[MAX_CONTROLLERS];

	void updateMousePosition(LPARAM lParam);

	void updateGamepadConnected();

};

COMA_END