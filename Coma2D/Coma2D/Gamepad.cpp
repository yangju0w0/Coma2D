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

void Gamepad::Update(double deltaTime)
{
	DWORD result;
	if (connected)
	{
		result = XInputGetState(controllerIndex, &state);
		if (result == ERROR_DEVICE_NOT_CONNECTED)
		{
			connected = false;
			DispatchEvent(new GamepadEvent(GamepadEvent::DISCONNECTED, this));
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
		DispatchEvent(new GamepadEvent(GamepadEvent::CONNECTED, this));
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
			DispatchEvent(new GamepadEvent(GamepadEvent::BUTTON_DOWN, this, indexData[i], left, !left));
			if (!buttonArray[i])
				DispatchEvent(new GamepadEvent(GamepadEvent::BUTTON_CLICK, this, indexData[i], left, !left));
			buttonArray[i] = true;
		}
		else if (buttonArray[i])
		{
			DispatchEvent(new GamepadEvent(GamepadEvent::BUTTON_UP, this, indexData[i], left, !left));
			buttonArray[i] = false;
		}
	}

	if (getLeftTrigger() != 0)
		DispatchEvent(new GamepadEvent(GamepadEvent::TRIGGER_INPUT, this, 0, true, false));
	if (getRightTrigger() != 0)
		DispatchEvent(new GamepadEvent(GamepadEvent::TRIGGER_INPUT, this, 0, false, true));
	if (getLeftThumbX() != 0 || getLeftThumbY() != 0)
		DispatchEvent(new GamepadEvent(GamepadEvent::THUMB_INPUT, this, GAMEPAD_LEFT_THUMB, true, false));
	if (getRightThumbX() != 0 || getRightThumbY() != 0)
		DispatchEvent(new GamepadEvent(GamepadEvent::THUMB_INPUT, this, GAMEPAD_RIGHT_THUMB, false, true));
}