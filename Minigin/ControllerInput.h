#pragma once	

#include "windows.h"
#include "XInput.h"

namespace dae
{
	class ControllerInput final
	{
	public:
		enum class Button 
		{
			DPadUp,
			DPadDown,
			DPadLeft,
			DPadRight,
			Start,
			Back,
			LeftThumb,
			RightThumb,
			LeftShoulder,
			RightShoulder,
			A ,
			B ,
			X ,
			Y ,
		};
		ControllerInput();
		~ControllerInput();
		void Update();
		bool IsButtonPressed( Button button) const;
		bool IsButtonHeld(Button button) const;
		bool IsButtonReleased(Button button) const;
		
		
	private:
		WORD GetButtonFlag(Button button) const;
		XINPUT_STATE currentState;
		XINPUT_STATE previousState;
		DWORD m_controllerIndex;
		unsigned int buttonsPressedThisFrame;
		unsigned int buttonsReleasedThisFrame;
	};
}