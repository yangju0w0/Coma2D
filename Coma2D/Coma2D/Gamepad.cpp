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
* filename	Gamepad.cpp
* fileinfo	게임패드 객체 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "Gamepad.h"
#include "GamepadEvent.h"

COMA_USING_NS

Gamepad::Gamepad(UINT controllerIndex)
:controllerIndex(0), vibrateTimeLeft(0), vibrateTimeRight(0), vibrateReadyTime(0), connected(false)
{
	this->controllerIndex = controllerIndex;
	if (controllerIndex > MAX_CONTROLLERS-1)
		this->controllerIndex = MAX_CONTROLLERS-1;

	for (int i = 0; i < 14; i++)
	{
		buttonArray[i] = false;
	}
	indexData = new DWORD[14] { GAMEPAD_DPAD_UP, GAMEPAD_DPAD_DOWN, GAMEPAD_DPAD_LEFT, GAMEPAD_DPAD_RIGHT, GAMEPAD_BACK_BUTTON, GAMEPAD_LEFT_THUMB, GAMEPAD_LEFT_SHOULDER, GAMEPAD_START_BUTTON, GAMEPAD_RIGHT_THUMB, GAMEPAD_RIGHT_SHOULDER, GAMEPAD_A, GAMEPAD_B, GAMEPAD_X, GAMEPAD_Y };
}


Gamepad::~Gamepad()
{
	delete[] indexData;
}

void Gamepad::update(double deltaTime)
{
	DWORD result;
	if (connected)
	{
		result = XInputGetState(controllerIndex, &state);
		if (result == ERROR_DEVICE_NOT_CONNECTED)
		{
			connected = false;
			dispatchEvent(new GamepadEvent(GamepadEvent::DISCONNECTED, this));
		}
	}
	createEvent();
	vibrateController(deltaTime);
}
void Gamepad::updateConntected()
{
	DWORD result;
	result = XInputGetState(controllerIndex, &state);
	if (result == ERROR_SUCCESS)
	{
		connected = true;
		dispatchEvent(new GamepadEvent(GamepadEvent::CONNECTED, this));
	}
	else
		connected = false;
}

BYTE Gamepad::getRawLeftTrigger()
{
	BYTE value = state.Gamepad.bLeftTrigger;
	if (value > GAMEPAD_TRIGGER_DEADZONE) 
		value = (value - GAMEPAD_TRIGGER_DEADZONE) * 255 / (255 - GAMEPAD_TRIGGER_DEADZONE);
	else
		value = 0;
	return value;
}
BYTE Gamepad::getRawRightTrigger()
{
	BYTE value = state.Gamepad.bRightTrigger;
	if (value > GAMEPAD_TRIGGER_DEADZONE)
		value = (value - GAMEPAD_TRIGGER_DEADZONE) * 255 / (255 - GAMEPAD_TRIGGER_DEADZONE);
	else
		value = 0;
	return value;
}
SHORT Gamepad::getRawLeftThumbX()
{
	int value = state.Gamepad.sThumbLX;
	if (value > GAMEPAD_THUMBSTICK_DEADZONE)
		value = (value - GAMEPAD_THUMBSTICK_DEADZONE) * 32767 / (32767 - GAMEPAD_THUMBSTICK_DEADZONE);
	else if (value < -GAMEPAD_THUMBSTICK_DEADZONE)
		value = (value + GAMEPAD_THUMBSTICK_DEADZONE) * 32767 / (32767 - GAMEPAD_THUMBSTICK_DEADZONE);
	else
		value = 0;
	return (SHORT)value;
}
SHORT Gamepad::getRawLeftThumbY()
{
	int value = state.Gamepad.sThumbLY;
	if (value > GAMEPAD_THUMBSTICK_DEADZONE)
		value = (value - GAMEPAD_THUMBSTICK_DEADZONE) * 32767 / (32767 - GAMEPAD_THUMBSTICK_DEADZONE);
	else if (value < -GAMEPAD_THUMBSTICK_DEADZONE)
		value = (value + GAMEPAD_THUMBSTICK_DEADZONE) * 32767 / (32767 - GAMEPAD_THUMBSTICK_DEADZONE);
	else
		value = 0;
	return (SHORT)value;
}
SHORT Gamepad::getRawRightThumbX()
{
	int value = state.Gamepad.sThumbRX;
	if (value > GAMEPAD_THUMBSTICK_DEADZONE)
		value = (value - GAMEPAD_THUMBSTICK_DEADZONE) * 32767 / (32767 - GAMEPAD_THUMBSTICK_DEADZONE);
	else if (value < -GAMEPAD_THUMBSTICK_DEADZONE)
		value = (value + GAMEPAD_THUMBSTICK_DEADZONE) * 32767 / (32767 - GAMEPAD_THUMBSTICK_DEADZONE);
	else
		value = 0;
	return (SHORT)value;
}
SHORT Gamepad::getRawRightThumbY()
{
	int value = state.Gamepad.sThumbRY;
	if (value > GAMEPAD_THUMBSTICK_DEADZONE)
		value = (value - GAMEPAD_THUMBSTICK_DEADZONE) * 32767 / (32767 - GAMEPAD_THUMBSTICK_DEADZONE);
	else if (value < -GAMEPAD_THUMBSTICK_DEADZONE)
		value = (value + GAMEPAD_THUMBSTICK_DEADZONE) * 32767 / (32767 - GAMEPAD_THUMBSTICK_DEADZONE);
	else
		value = 0;
	return (SHORT)value;
}

void Gamepad::vibrateLeft(float speed, float time)
{
	vibrateTimeLeft = time;
	vibration.wLeftMotorSpeed = speed*65535;
}
void Gamepad::vibrateRight(float speed, float time)
{
	vibrateTimeRight = time;
	vibration.wRightMotorSpeed = speed*65535;
}
void Gamepad::vibrateRawLeft(WORD speed, float time)
{
	vibrateTimeLeft = time;
	vibration.wLeftMotorSpeed = speed;
}
void Gamepad::vibrateRawRight(WORD speed, float time)
{
	vibrateTimeRight = time;
	vibration.wRightMotorSpeed = speed;
}

void Gamepad::vibrateController(double frameTime)
{
	if (!connected)
		return;

	if (vibrateReadyTime < 0.016f)
	{
		vibrateReadyTime += frameTime;
		return;
	}
	vibrateReadyTime = 0;

	vibrateTimeLeft -= frameTime;
	if (vibrateTimeLeft < 0)
	{
		vibrateTimeLeft = 0;
		vibration.wLeftMotorSpeed = 0;
	}
	vibrateTimeRight -= frameTime;
	if (vibrateTimeRight < 0)
	{
		vibrateTimeRight = 0;
		vibration.wRightMotorSpeed = 0;
	}
	XInputSetState(controllerIndex, &vibration);
}

void Gamepad::createEvent()
{
	if (!connected)
		return;
	for (int i = 0; i < 14; i++)
	{
		bool left = true;
		if (i > 6)
			left = false;
		if ((state.Gamepad.wButtons&indexData[i]) != 0)
		{
			dispatchEvent(new GamepadEvent(GamepadEvent::BUTTON_DOWN, this, indexData[i], left, !left));
			if (!buttonArray[i])
				dispatchEvent(new GamepadEvent(GamepadEvent::BUTTON_CLICK, this, indexData[i], left, !left));
			buttonArray[i] = true;
		}
		else if (buttonArray[i])
		{
			dispatchEvent(new GamepadEvent(GamepadEvent::BUTTON_UP, this, indexData[i], left, !left));
			buttonArray[i] = false;
		}
	}

	if (getLeftTrigger() != 0)
		dispatchEvent(new GamepadEvent(GamepadEvent::TRIGGER_INPUT, this, 0, true, false));
	if (getRightTrigger() != 0)
		dispatchEvent(new GamepadEvent(GamepadEvent::TRIGGER_INPUT, this, 0, false, true));
	if (getLeftThumbX() != 0 || getLeftThumbY() != 0)
		dispatchEvent(new GamepadEvent(GamepadEvent::THUMB_INPUT, this, GAMEPAD_LEFT_THUMB, true, false));
	if (getRightThumbX() != 0 || getRightThumbY() != 0)
		dispatchEvent(new GamepadEvent(GamepadEvent::THUMB_INPUT, this, GAMEPAD_RIGHT_THUMB, false, true));
}