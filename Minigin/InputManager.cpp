#include <SDL3/SDL.h>
#include "InputManager.h"


void dae::InputManager::Init()
{
	m_KeyboardState = SDL_GetKeyboardState(NULL);
	m_Controllers.push_back(std::make_unique<ControllerInput>());
}
bool dae::InputManager::ProcessInput(float deltaTime)
{
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

	for (const auto& [key, command] : m_KeyboardBindings)
	{
		if (m_KeyboardState[SDL_GetScancodeFromKey(key.first, NULL)] && key.second == ButtonState::Held) 
			command.get()->Execute(deltaTime);
	}

	for (auto& controller : m_Controllers)
	{
		controller->Update();
	}

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

void dae::InputManager::BindCommand(SDL_Keycode key, ButtonState buttonState, std::unique_ptr<Command> command)
{
	m_KeyboardBindings.insert({ std::make_pair(key, buttonState), std::move(command) });
}

void dae::InputManager::BindCommand(ControllerInput::Button button, ButtonState buttonState, std::unique_ptr<Command> command)
{
	m_ControllerBindings.insert({ {button, buttonState}, std::move(command) });

}

void dae::InputManager::UnbindCommand(ControllerInput::Button button, ButtonState buttonState)
{
	m_ControllerBindings.erase({ button, buttonState });

}

void dae::InputManager::UnbindCommand(SDL_Keycode key, ButtonState buttonState)
{
	m_KeyboardBindings.erase(std::make_pair(key, buttonState));
}

