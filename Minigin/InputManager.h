#pragma once
#include "Singleton.h"
#include <vector>
#include <memory>
#include <map>
#include "Commands/Command.h"
#include "ControllerInput.h"
namespace dae
{
	// Handles SDL keyboard input and controller input, uses the Command pattern to bind input actions to gameplay commands.
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

		bool IsPressed(ControllerInput::Button button) const;
		bool IsHeld(ControllerInput::Button button) const;
		bool IsReleased(ControllerInput::Button button) const;

		void BindCommand(SDL_Keycode key, ButtonState buttonState, std::unique_ptr<Command> command);
		void BindCommand(ControllerInput::Button button, ButtonState buttonState, std::unique_ptr<Command> command);
		void UnbindCommand(ControllerInput::Button button, ButtonState buttonState);
		void UnbindCommand(SDL_Keycode key, ButtonState buttonState);
		void ClearCommands();
		void Init();
	private:
		// Keyboard bindings: key + button state -> command.
		std::map < std::pair<SDL_Keycode, ButtonState>, std::unique_ptr<Command>> m_KeyboardBindings{};
		std::vector<std::unique_ptr<ControllerInput>> m_Controllers{};
		// Controller bindings: button + button state -> command.
		std::map<std::pair<ControllerInput::Button, ButtonState>, std::unique_ptr<Command>> m_ControllerBindings{};
		const bool* m_KeyboardState{};
	};

}
