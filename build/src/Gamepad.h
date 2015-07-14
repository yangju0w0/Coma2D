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
* filename	Gamepad.h
* fileinfo	게임패드 객체 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#pragma once
#include "EventDispatcher.h"
#include <Xinput.h>

COMA_NS_BEGIN

const short GAMEPAD_THUMBSTICK_DEADZONE = (short)(0.20f * 0X7FFF);
const short GAMEPAD_TRIGGER_DEADZONE = 20;
const DWORD MAX_CONTROLLERS = 4;

const DWORD GAMEPAD_DPAD_UP = 0x0001;
const DWORD GAMEPAD_DPAD_DOWN = 0x0002;
const DWORD GAMEPAD_DPAD_LEFT = 0x0004;
const DWORD GAMEPAD_DPAD_RIGHT = 0x0008;
const DWORD GAMEPAD_START_BUTTON = 0x0010;
const DWORD GAMEPAD_BACK_BUTTON = 0x0020;
const DWORD GAMEPAD_LEFT_THUMB = 0x0040;
const DWORD GAMEPAD_RIGHT_THUMB = 0x0080;
const DWORD GAMEPAD_LEFT_SHOULDER = 0x0100;
const DWORD GAMEPAD_RIGHT_SHOULDER = 0x0200;
const DWORD GAMEPAD_A = 0x1000;
const DWORD GAMEPAD_B = 0x2000;
const DWORD GAMEPAD_X = 0x4000;
const DWORD GAMEPAD_Y = 0x8000;

class Gamepad :
	public EventDispatcher
{
public:
	Gamepad(UINT controllerIndex);
	virtual ~Gamepad();

public:
	void update(double deltaTime);
	void updateConntected();


	void vibrateLeft(float speed, float time);
	void vibrateRight(float speed, float time);
	void vibrateRawLeft(WORD speed, float time);
	void vibrateRawRight(WORD speed, float time);

	bool isConnected(){ return connected; }
	XINPUT_STATE getState(){ return state; }

	bool getDpadUp()		{ return ((state.Gamepad.wButtons&GAMEPAD_DPAD_UP) != 0); }
	bool getDpadDown()		{ return ((state.Gamepad.wButtons&GAMEPAD_DPAD_DOWN) != 0); }
	bool getDpadLeft()		{ return ((state.Gamepad.wButtons&GAMEPAD_DPAD_LEFT) != 0); }
	bool getDpadRight()		{ return ((state.Gamepad.wButtons&GAMEPAD_DPAD_RIGHT) != 0); }
	bool getStartButton()	{ return ((state.Gamepad.wButtons&GAMEPAD_START_BUTTON) != 0); }
	bool getBackButton()	{ return ((state.Gamepad.wButtons&GAMEPAD_BACK_BUTTON) != 0); }
	bool getLeftThumb()		{ return ((state.Gamepad.wButtons&GAMEPAD_LEFT_THUMB) != 0); }
	bool getRightThumb()	{ return ((state.Gamepad.wButtons&GAMEPAD_RIGHT_THUMB) != 0); }
	bool getLeftShoulder()	{ return ((state.Gamepad.wButtons&GAMEPAD_LEFT_SHOULDER) != 0); }
	bool getRightShoulder()	{ return ((state.Gamepad.wButtons&GAMEPAD_RIGHT_SHOULDER) != 0); }
	bool getA()				{ return ((state.Gamepad.wButtons&GAMEPAD_A) != 0); }
	bool getB()				{ return ((state.Gamepad.wButtons&GAMEPAD_B) != 0); }
	bool getX()				{ return ((state.Gamepad.wButtons&GAMEPAD_X) != 0); }
	bool getY()				{ return ((state.Gamepad.wButtons&GAMEPAD_Y) != 0); }

	BYTE	getRawLeftTrigger();
	BYTE	getRawRightTrigger();
	SHORT	getRawLeftThumbX();
	SHORT	getRawLeftThumbY();
	SHORT	getRawRightThumbX();
	SHORT	getRawRightThumbY();

	float	getLeftTrigger()		{ return getRawLeftTrigger() / 255.f; }
	float	getRightTrigger()		{ return getRawRightTrigger() / 255.f; }
	float	getLeftThumbX()			{ return getRawLeftThumbX() / 32767.f; }
	float	getLeftThumbY()			{ return getRawLeftThumbY() / 32767.f; }
	float	getRightThumbX()		{ return getRawRightThumbX() / 32767.f; }
	float	getRightThumbY()		{ return getRawRightThumbY() / 32767.f; }
	Point	getLeftThumbPosition()	{ return Point{ getLeftThumbX(), getLeftThumbY() }; }
	Point	getRightThumbPosition()	{ return Point{ getRightThumbX(), getRightThumbY() }; }

private:
	void vibrateController(double frameTime);
	void createEvent();

private:
	UINT				controllerIndex;

	XINPUT_STATE        state;
	XINPUT_VIBRATION    vibration;
	float               vibrateTimeLeft;
	float               vibrateTimeRight;
	bool                connected;
	float				vibrateReadyTime; // 진동을 줄 때, 일정 시간정도의 대기시간이 필요하다.

	bool				buttonArray[14];
	DWORD*				indexData;

};

COMA_END