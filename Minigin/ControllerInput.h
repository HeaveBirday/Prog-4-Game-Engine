#pragma once	


#include <memory>

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
	

		class Impl;
		std::unique_ptr<Impl> pImpl;
	};
}