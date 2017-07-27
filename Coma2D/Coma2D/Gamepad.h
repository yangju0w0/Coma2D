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
	explicit Gamepad(UINT controllerIndex);
	virtual ~Gamepad();

public:
	void Update(double deltaTime);
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
	bool GetX()				{ return ((state.Gamepad.wButtons&GAMEPAD_X) != 0); }
	bool GetY()				{ return ((state.Gamepad.wButtons&GAMEPAD_Y) != 0); }

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