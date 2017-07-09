/*
* The MIT License (MIT)
*
* Copyright (c) 2015 Heonyang Ju
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*
* MIT 라이선스 (MIT)
*
* 저작권(C) 2015 주헌양
*
* 이 문서에 의해, 아래의 조건을 따르는 한 제한 없이 소프트웨어를 다루기 위해
* 소프트웨어의 복사본과 관련된 문서("소프트웨어")를 획득하는 사람에게 변경의
* 자유가 허용 된다. 제한 없음은 사용, 복제, 수정, 통합, 공포, 배포, 하위 라이선스
* 그리고/또는 소프트웨어의 복사본의 판매와 공급된 소프트웨어를 가진 사람에게
* 그렇게 할 수 있도록 허가하는 권한에 제한 없음을 포함 한다.
*
* 위의 저작권 표시와 이 허가 표시는 모든 소프트웨어의 복사본 또는 상당한 일부분에
* 포함되어야 한다.
*
* 이 소프트웨어는 특정한 목적과 침해 없음을 위해 상업성, 적합성의 보증에만
* 국한되는 것이 아닌 명시적 또는 묵시적인 어떠한 종류의 보증없이 "있는 그대로"
* 제공 된다. 어떠한 경우에도 저작자 또는 저작권자는 소프트웨어의 관계 안과 밖
* 또는 소프트웨어의 사용 또는 거래로부터 발생하는 계약, 불법 행위 또는 기타 수행에
* 대해, 어떠한 소송, 손해 또는 다른 책무에 대해서도 책임이 없다.
*
* 본문은 MIT라이선스 원문의 번역본이며, 모든 해석은 원문에 기초한다.
*/

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