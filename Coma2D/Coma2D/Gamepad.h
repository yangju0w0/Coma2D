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
	explicit Gamepad(UINT controllerIndex_);
	virtual ~Gamepad();

public:
	void Update(double deltaTime);
	void UpdateConntected();

	void VibrateLeft(float speed, float time);
	void VibrateRight(float speed, float time);
	void VibrateRawLeft(WORD speed, float time);
	void VibrateRawRight(WORD speed, float time);

	bool IsConnected() const { return connected_; }
	XINPUT_STATE GetState() const { return state_; }

	bool GetDpadUp() const { return ((state_.Gamepad.wButtons&GAMEPAD_DPAD_UP) != 0); }
	bool GetDpadDown() const { return ((state_.Gamepad.wButtons&GAMEPAD_DPAD_DOWN) != 0); }
	bool GetDpadLeft() const { return ((state_.Gamepad.wButtons&GAMEPAD_DPAD_LEFT) != 0); }
	bool GetDpadRight()	const { return ((state_.Gamepad.wButtons&GAMEPAD_DPAD_RIGHT) != 0); }
	bool GetStartButton() const { return ((state_.Gamepad.wButtons&GAMEPAD_START_BUTTON) != 0); }
	bool GetBackButton() const { return ((state_.Gamepad.wButtons&GAMEPAD_BACK_BUTTON) != 0); }
	bool GetLeftThumb() const { return ((state_.Gamepad.wButtons&GAMEPAD_LEFT_THUMB) != 0); }
	bool GetRightThumb() const { return ((state_.Gamepad.wButtons&GAMEPAD_RIGHT_THUMB) != 0); }
	bool GetLeftShoulder() const { return ((state_.Gamepad.wButtons&GAMEPAD_LEFT_SHOULDER) != 0); }
	bool GetRightShoulder() const { return ((state_.Gamepad.wButtons&GAMEPAD_RIGHT_SHOULDER) != 0); }
	bool GetA() const { return ((state_.Gamepad.wButtons&GAMEPAD_A) != 0); }
	bool GetB() const { return ((state_.Gamepad.wButtons&GAMEPAD_B) != 0); }
	bool GetX() const { return ((state_.Gamepad.wButtons&GAMEPAD_X) != 0); }
	bool GetY() const { return ((state_.Gamepad.wButtons&GAMEPAD_Y) != 0); }

	BYTE GetRawLeftTrigger();
	BYTE GetRawRightTrigger();
	SHORT GetRawLeftThumbX();
	SHORT GetRawLeftThumbY();
	SHORT GetRawRightThumbX();
	SHORT GetRawRightThumbY();

	float GetLeftTrigger() { return GetRawLeftTrigger() / 255.f; }
	float GetRightTrigger() { return GetRawRightTrigger() / 255.f; }
	float GetLeftThumbX() { return GetRawLeftThumbX() / 32767.f; }
	float GetLeftThumbY() { return GetRawLeftThumbY() / 32767.f; }
	float GetRightThumbX() { return GetRawRightThumbX() / 32767.f; }
	float GetRightThumbY() { return GetRawRightThumbY() / 32767.f; }
	Point GetLeftThumbPosition() { return Point{ GetLeftThumbX(), GetLeftThumbY() }; }
	Point GetRightThumbPosition() { return Point{ GetRightThumbX(), GetRightThumbY() }; }

private:
	void VibrateController(double frameTime);
	void CreateEvent();

private:
	UINT controllerIndex_;

	XINPUT_STATE state_;
	XINPUT_VIBRATION vibration_;
	float vibrateTimeLeft_;
	float vibrateTimeRight_;
	bool connected_;
	float vibrateReadyTime_; // 진동을 줄 때, 일정 시간정도의 대기시간이 필요하다.

	bool buttonArray_[14];
	DWORD* indexData_;

};

COMA_END