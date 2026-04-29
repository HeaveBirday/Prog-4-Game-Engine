#pragma once
#include "Singleton.h"
#include <vector>
#include <memory>
#include <map>
#include "Commands/Command.h"
#include "ControllerInput.h"
namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput(float deltaTime);
		enum class ButtonState
		{
			Pressed,
			Held,
			Released

		};

		void BindCommand(SDL_Keycode key, ButtonState buttonState, std::unique_ptr<Command> command);
		void BindCommand(ControllerInput::Button button, ButtonState buttonState, std::unique_ptr<Command> command);
		void UnbindCommand(ControllerInput::Button button, ButtonState buttonState);
		void UnbindCommand(SDL_Keycode key, ButtonState buttonState);
		void Init();
	private:

		std::map < std::pair<SDL_Keycode, ButtonState>, std::unique_ptr<Command>> m_KeyboardBindings{};
		std::vector<std::unique_ptr<ControllerInput>> m_Controllers{};
		std::map<std::pair<ControllerInput::Button, ButtonState>, std::unique_ptr<Command>> m_ControllerBindings{};
		const bool* m_KeyboardState{};
	};

}
