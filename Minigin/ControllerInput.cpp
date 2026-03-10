#include <utility>
#include "ControllerInput.h"

dae::ControllerInput::ControllerInput() : m_controllerIndex(0),
currentState{}
, previousState{}
, buttonsPressedThisFrame{}
, buttonsReleasedThisFrame{}
{
}

dae::ControllerInput::~ControllerInput()
{
}

void dae::ControllerInput::Update()
{
	CopyMemory(&previousState, &currentState, sizeof(XINPUT_STATE));
	ZeroMemory(&currentState, sizeof(XINPUT_STATE));
	XInputGetState(m_controllerIndex, &currentState);
	auto buttonChanges = currentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
	buttonsPressedThisFrame = buttonChanges & currentState.Gamepad.wButtons;
	buttonsReleasedThisFrame = buttonChanges & (~currentState.Gamepad.wButtons);
}

bool dae::ControllerInput::IsButtonPressed(Button button) const
{
	return (buttonsPressedThisFrame & GetButtonFlag(button)) != 0;
}

bool dae::ControllerInput::IsButtonHeld( Button button) const
{
	return (currentState.Gamepad.wButtons & GetButtonFlag(button)) != 0;
}

bool dae::ControllerInput::IsButtonReleased( Button button) const
{
	return (buttonsReleasedThisFrame & GetButtonFlag(button)) != 0;
}

WORD dae::ControllerInput::GetButtonFlag(Button button) const
{
	
	switch (button)
	{
	case dae::ControllerInput::Button::DPadUp:
		return XINPUT_GAMEPAD_DPAD_UP;
		break;
	case dae::ControllerInput::Button::DPadDown:
		return XINPUT_GAMEPAD_DPAD_DOWN;
		break;
	case dae::ControllerInput::Button::DPadLeft:
		return XINPUT_GAMEPAD_DPAD_LEFT;
		break;
	case dae::ControllerInput::Button::DPadRight:
		return XINPUT_GAMEPAD_DPAD_RIGHT;
		break;
	case dae::ControllerInput::Button::Start:
		return XINPUT_GAMEPAD_START;
		break;
	case dae::ControllerInput::Button::Back:
		return XINPUT_GAMEPAD_BACK;
		break;
	case dae::ControllerInput::Button::LeftThumb:
		return XINPUT_GAMEPAD_LEFT_THUMB;
		break;
	case dae::ControllerInput::Button::RightThumb:
		return XINPUT_GAMEPAD_RIGHT_THUMB;
		break;
	case dae::ControllerInput::Button::LeftShoulder:
		return XINPUT_GAMEPAD_LEFT_SHOULDER;
		break;
	case dae::ControllerInput::Button::RightShoulder:
		return XINPUT_GAMEPAD_RIGHT_SHOULDER;
		break;
	case dae::ControllerInput::Button::A:
		return XINPUT_GAMEPAD_A;
		break;
	case dae::ControllerInput::Button::B:
		return XINPUT_GAMEPAD_B;
		break;
	case dae::ControllerInput::Button::X:
		return XINPUT_GAMEPAD_X;
		break;
	case dae::ControllerInput::Button::Y:
		return XINPUT_GAMEPAD_Y;
		break;
	default:
		break;
	}
	return 0;
}
