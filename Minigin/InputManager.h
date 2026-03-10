#pragma once
#include "Singleton.h"
#include <vector>
#include <memory>
#include <map>
#include "Command.h"
namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		enum class ButtonState
		{
			Pressed,
			Held,
			Released

		};

		void BindCommand(SDL_Keycode key, ButtonState buttonState, std::unique_ptr<Command> command);
		void UnbindCommand(SDL_Keycode key, ButtonState buttonState);
		void Init();
	private:

		std::map < std::pair<SDL_Keycode, ButtonState>, std::unique_ptr<Command>> m_KeyboardBindings{};

		const bool* m_KeyboardState{};
	};

}
