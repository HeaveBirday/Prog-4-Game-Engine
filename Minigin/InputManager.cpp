#include <SDL3/SDL.h>
#include "InputManager.h"

// Why does this class exist? Because we want to decouple input handling from the rest of the game logic, and also to allow for easy rebinding of controls.
void dae::InputManager::Init()
{
	m_KeyboardState = SDL_GetKeyboardState(NULL);
	m_Controllers.push_back(std::make_unique<ControllerInput>());
}
// The ProcessInput function is responsible for polling SDL events and executing the corresponding commands based on the current input state. It also updates the state of the controllers and checks for held buttons.
bool dae::InputManager::ProcessInput(float deltaTime)
{
	// Poll SDL events and execute commands based on the event type and the current input state.
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_EVENT_QUIT)
		{
			return false;
		}
		if (e.type == SDL_EVENT_KEY_DOWN)
		{
			for (const auto& [key, buttonState] : m_KeyboardBindings)
			{
				if (key.first == e.key.key && key.second == ButtonState::Pressed)
				{
					m_KeyboardBindings[key]->Execute(deltaTime);
				}
			}

		}
		if (e.type == SDL_EVENT_KEY_UP)
		{
			for (const auto& [key, buttonState] : m_KeyboardBindings)
			{
				if (key.first == e.key.key && key.second == ButtonState::Released)
				{
					m_KeyboardBindings[key]->Execute(deltaTime);
				}
			}
		}
		
		if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
		{
		
		}
	}
	// Check for held buttons and execute the corresponding commands.
	for (const auto& [key, command] : m_KeyboardBindings)
	{
		if (m_KeyboardState[SDL_GetScancodeFromKey(key.first, NULL)] && key.second == ButtonState::Held) 
			command.get()->Execute(deltaTime);
	}
	// Update the state of the controllers and check for held buttons.
	for (auto& controller : m_Controllers)
	{
		controller->Update();
	}
	// Check for held buttons on the controller and execute the corresponding commands.
	for (const auto& [binding, command] : m_ControllerBindings)
	{
		if (binding.second == ButtonState::Pressed && m_Controllers[0]->IsButtonPressed(binding.first))
		{
			command->Execute(deltaTime);
		}
		if (binding.second == ButtonState::Held && m_Controllers[0]->IsButtonHeld(binding.first))
		{
			command->Execute(deltaTime);
		}
		if (binding.second == ButtonState::Released && m_Controllers[0]->IsButtonReleased(binding.first))
		{
			command->Execute(deltaTime);
		}
	}
	return true;
}
// The BindCommand functions allow you to bind a command to a specific key or controller button and button state. 
void dae::InputManager::BindCommand(SDL_Keycode key, ButtonState buttonState, std::unique_ptr<Command> command)
{
	m_KeyboardBindings.insert({ std::make_pair(key, buttonState), std::move(command) });
}
void dae::InputManager::BindCommand(ControllerInput::Button button, ButtonState buttonState, std::unique_ptr<Command> command)
{
	m_ControllerBindings.insert({ {button, buttonState}, std::move(command) });

}
// The UnbindCommand functions allow you to unbind a command from a specific key or controller button and button state.
void dae::InputManager::UnbindCommand(ControllerInput::Button button, ButtonState buttonState)
{
	m_ControllerBindings.erase({ button, buttonState });

}

void dae::InputManager::UnbindCommand(SDL_Keycode key, ButtonState buttonState)
{
	m_KeyboardBindings.erase(std::make_pair(key, buttonState));
}
// The ClearCommands function clears all the command bindings from both the keyboard and controller.
void dae::InputManager::ClearCommands()
{
	m_KeyboardBindings.clear();
	m_ControllerBindings.clear();
}

bool dae::InputManager::IsPressed(ControllerInput::Button button) const
{
	return !m_Controllers.empty() && m_Controllers[0]->IsButtonPressed(button);
}

bool dae::InputManager::IsHeld(ControllerInput::Button button) const
{
	return !m_Controllers.empty() && m_Controllers[0]->IsButtonHeld(button);
}

bool dae::InputManager::IsReleased(ControllerInput::Button button) const
{
	return !m_Controllers.empty() && m_Controllers[0]->IsButtonReleased(button);
}