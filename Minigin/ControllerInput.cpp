#include <utility>
#include "ControllerInput.h"

#if defined (_WIN32)
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include "XInput.h"
#endif

class dae::ControllerInput::Impl
{
public:
	Impl() : m_controllerIndex(0),
		currentState{}
		, previousState{}
		, buttonsPressedThisFrame{}
		, buttonsReleasedThisFrame{}
	{

	}
	void Update()
	{
		CopyMemory(&previousState, &currentState, sizeof(XINPUT_STATE));
		ZeroMemory(&currentState, sizeof(XINPUT_STATE));
		XInputGetState(m_controllerIndex, &currentState);
		auto buttonChanges = currentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
		buttonsPressedThisFrame = buttonChanges & currentState.Gamepad.wButtons;
		buttonsReleasedThisFrame = buttonChanges & (~currentState.Gamepad.wButtons);
	}
	bool IsButtonPressed(Button button) const
	{
		return (buttonsPressedThisFrame & GetButtonFlag(button)) != 0;
	}
	bool IsButtonHeld(Button button) const
	{
		return (currentState.Gamepad.wButtons & GetButtonFlag(button)) != 0;

	}
	bool IsButtonReleased(Button button) const
	{
		return (buttonsReleasedThisFrame & GetButtonFlag(button)) != 0;

	}

private:
	WORD GetButtonFlag(Button button) const
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
	XINPUT_STATE currentState;
	XINPUT_STATE previousState;
	DWORD m_controllerIndex;
	unsigned int buttonsPressedThisFrame;
	unsigned int buttonsReleasedThisFrame;

};

dae::ControllerInput::ControllerInput() : pImpl(std::make_unique<Impl>())
{
}

dae::ControllerInput::~ControllerInput() 
{

}

void dae::ControllerInput::Update()
{
	pImpl->Update();
}

bool dae::ControllerInput::IsButtonPressed(Button button) const
{
	return pImpl->IsButtonPressed(button);
}

bool dae::ControllerInput::IsButtonHeld( Button button) const
{
	return pImpl->IsButtonHeld(button);
}

bool dae::ControllerInput::IsButtonReleased( Button button) const
{
	return pImpl->IsButtonReleased(button);
}
