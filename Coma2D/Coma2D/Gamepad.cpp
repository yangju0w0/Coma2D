/*
* filename	Gamepad.cpp
* fileinfo	게임패드 객체 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "Gamepad.h"
#include "GamepadEvent.h"

COMA_USING_NS

Gamepad::Gamepad(UINT controllerIndex_)
	:controllerIndex_(0), vibrateTimeLeft_(0), vibrateTimeRight_(0), vibrateReadyTime_(0), connected_(false)
{
	this->controllerIndex_ = controllerIndex_;
	if (controllerIndex_ > MAX_CONTROLLERS - 1)
	{
		this->controllerIndex_ = MAX_CONTROLLERS - 1;
	}

	for (int i = 0; i < 14; i++)
	{
		buttonArray_[i] = false;
	}
	indexData_ = new DWORD[14]{ GAMEPAD_DPAD_UP, GAMEPAD_DPAD_DOWN, GAMEPAD_DPAD_LEFT, GAMEPAD_DPAD_RIGHT, GAMEPAD_BACK_BUTTON, GAMEPAD_LEFT_THUMB, GAMEPAD_LEFT_SHOULDER, GAMEPAD_START_BUTTON, GAMEPAD_RIGHT_THUMB, GAMEPAD_RIGHT_SHOULDER, GAMEPAD_A, GAMEPAD_B, GAMEPAD_X, GAMEPAD_Y };
}

Gamepad::~Gamepad()
{
	delete[] indexData_;
}

void Gamepad::Update(double deltaTime)
{
	DWORD result;
	if (connected_)
	{
		result = XInputGetState(controllerIndex_, &state_);
		if (result == ERROR_DEVICE_NOT_CONNECTED)
		{
			connected_ = false;
			DispatchEvent(new GamepadEvent(GamepadEvent::DISCONNECTED, this));
		}
	}
	CreateEvent();
	VibrateController(deltaTime);
}

void Gamepad::UpdateConntected()
{
	DWORD result;
	result = XInputGetState(controllerIndex_, &state_);
	if (result == ERROR_SUCCESS)
	{
		connected_ = true;
		DispatchEvent(new GamepadEvent(GamepadEvent::CONNECTED, this));
	}
	else
	{
		connected_ = false;
	}
}

BYTE Gamepad::GetRawLeftTrigger()
{
	BYTE value = state_.Gamepad.bLeftTrigger;
	if (value > GAMEPAD_TRIGGER_DEADZONE)
	{
		value = (value - GAMEPAD_TRIGGER_DEADZONE) * 255 / (255 - GAMEPAD_TRIGGER_DEADZONE);
	}
	else
	{
		value = 0;
	}
	return value;
}

BYTE Gamepad::GetRawRightTrigger()
{
	BYTE value = state_.Gamepad.bRightTrigger;
	if (value > GAMEPAD_TRIGGER_DEADZONE)
	{
		value = (value - GAMEPAD_TRIGGER_DEADZONE) * 255 / (255 - GAMEPAD_TRIGGER_DEADZONE);
	}
	else
	{
		value = 0;
	}
	return value;
}

SHORT Gamepad::GetRawLeftThumbX()
{
	int value = state_.Gamepad.sThumbLX;

	if (value > GAMEPAD_THUMBSTICK_DEADZONE)
	{
		value = (value - GAMEPAD_THUMBSTICK_DEADZONE) * 32767 / (32767 - GAMEPAD_THUMBSTICK_DEADZONE);
	}
	else if (value < -GAMEPAD_THUMBSTICK_DEADZONE)
	{
		value = (value + GAMEPAD_THUMBSTICK_DEADZONE) * 32767 / (32767 - GAMEPAD_THUMBSTICK_DEADZONE);
	}
	else
	{
		value = 0;
	}

	return (SHORT)value;
}

SHORT Gamepad::GetRawLeftThumbY()
{
	int value = state_.Gamepad.sThumbLY;
	if (value > GAMEPAD_THUMBSTICK_DEADZONE)
	{
		value = (value - GAMEPAD_THUMBSTICK_DEADZONE) * 32767 / (32767 - GAMEPAD_THUMBSTICK_DEADZONE);
	}
	else if (value < -GAMEPAD_THUMBSTICK_DEADZONE)
	{
		value = (value + GAMEPAD_THUMBSTICK_DEADZONE) * 32767 / (32767 - GAMEPAD_THUMBSTICK_DEADZONE);
	}
	else
	{
		value = 0;
	}

	return (SHORT)value;
}

SHORT Gamepad::GetRawRightThumbX()
{
	int value = state_.Gamepad.sThumbRX;
	if (value > GAMEPAD_THUMBSTICK_DEADZONE)
	{
		value = (value - GAMEPAD_THUMBSTICK_DEADZONE) * 32767 / (32767 - GAMEPAD_THUMBSTICK_DEADZONE);
	}
	else if (value < -GAMEPAD_THUMBSTICK_DEADZONE)
	{
		value = (value + GAMEPAD_THUMBSTICK_DEADZONE) * 32767 / (32767 - GAMEPAD_THUMBSTICK_DEADZONE);
	}
	else
	{
		value = 0;
	}
	return (SHORT)value;
}
SHORT Gamepad::GetRawRightThumbY()
{
	int value = state_.Gamepad.sThumbRY;
	if (value > GAMEPAD_THUMBSTICK_DEADZONE)
	{
		value = (value - GAMEPAD_THUMBSTICK_DEADZONE) * 32767 / (32767 - GAMEPAD_THUMBSTICK_DEADZONE);
	}
	else if (value < -GAMEPAD_THUMBSTICK_DEADZONE)
	{
		value = (value + GAMEPAD_THUMBSTICK_DEADZONE) * 32767 / (32767 - GAMEPAD_THUMBSTICK_DEADZONE);
	}
	else
	{
		value = 0;
	}
	return (SHORT)value;
}

void Gamepad::VibrateLeft(float speed, float time)
{
	vibrateTimeLeft_ = time;
	vibration_.wLeftMotorSpeed = speed * 65535;
}
void Gamepad::VibrateRight(float speed, float time)
{
	vibrateTimeRight_ = time;
	vibration_.wRightMotorSpeed = speed * 65535;
}
void Gamepad::VibrateRawLeft(WORD speed, float time)
{
	vibrateTimeLeft_ = time;
	vibration_.wLeftMotorSpeed = speed;
}
void Gamepad::VibrateRawRight(WORD speed, float time)
{
	vibrateTimeRight_ = time;
	vibration_.wRightMotorSpeed = speed;
}

void Gamepad::VibrateController(double frameTime)
{
	if (!connected_)
		return;

	if (vibrateReadyTime_ < 0.016f)
	{
		vibrateReadyTime_ += frameTime;
		return;
	}
	vibrateReadyTime_ = 0;

	vibrateTimeLeft_ -= frameTime;
	if (vibrateTimeLeft_ < 0)
	{
		vibrateTimeLeft_ = 0;
		vibration_.wLeftMotorSpeed = 0;
	}
	vibrateTimeRight_ -= frameTime;
	if (vibrateTimeRight_ < 0)
	{
		vibrateTimeRight_ = 0;
		vibration_.wRightMotorSpeed = 0;
	}
	XInputSetState(controllerIndex_, &vibration_);
}

void Gamepad::CreateEvent()
{
	if (!connected_)
		return;
	for (int i = 0; i < 14; i++)
	{
		bool left = true;
		if (i > 6)
			left = false;
		if ((state_.Gamepad.wButtons&indexData_[i]) != 0)
		{
			DispatchEvent(new GamepadEvent(GamepadEvent::BUTTON_DOWN, this, indexData_[i], left, !left));
			if (!buttonArray_[i])
			{
				DispatchEvent(new GamepadEvent(GamepadEvent::BUTTON_CLICK, this, indexData_[i], left, !left));
			}
			buttonArray_[i] = true;
		}
		else if (buttonArray_[i])
		{
			DispatchEvent(new GamepadEvent(GamepadEvent::BUTTON_UP, this, indexData_[i], left, !left));
			buttonArray_[i] = false;
		}
	}

	if (GetLeftTrigger() != 0)
	{
		DispatchEvent(new GamepadEvent(GamepadEvent::TRIGGER_INPUT, this, 0, true, false));
	}
	if (GetRightTrigger() != 0)
	{
		DispatchEvent(new GamepadEvent(GamepadEvent::TRIGGER_INPUT, this, 0, false, true));
	}
	if (GetLeftThumbX() != 0 || GetLeftThumbY() != 0)
	{
		DispatchEvent(new GamepadEvent(GamepadEvent::THUMB_INPUT, this, GAMEPAD_LEFT_THUMB, true, false));
	}
	if (GetRightThumbX() != 0 || GetRightThumbY() != 0)
	{
		DispatchEvent(new GamepadEvent(GamepadEvent::THUMB_INPUT, this, GAMEPAD_RIGHT_THUMB, false, true));
	}
}