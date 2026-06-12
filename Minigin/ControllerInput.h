#pragma once	


#include <memory>

namespace dae
{
	// This class is responsible for handling controller input using XInput 
	// It keeps track of the current and previous state of the controller, as well as which buttons were pressed and released in the current frame
	// It provides methods to check if a specific button is currently pressed, held down, or released.
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
	
		// PIMPL pattern to hide implementation details and reduce compile-time dependencies
		class Impl;
		std::unique_ptr<Impl> pImpl;
	};
}