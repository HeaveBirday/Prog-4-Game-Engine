#include <SDL3/SDL.h>
#include "InputManager.h"
#include <backends/imgui_impl_sdl3.h>



void dae::InputManager::Init()
{
	m_KeyboardState = SDL_GetKeyboardState(NULL);
}
bool dae::InputManager::ProcessInput()
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
					m_KeyboardBindings[key]->Execute();
				}
			}

		}
		if (e.type == SDL_EVENT_KEY_UP)
		{
			for (const auto& [key, buttonState] : m_KeyboardBindings)
			{
				if (key.first == e.key.key && key.second == ButtonState::Released)
				{
					m_KeyboardBindings[key]->Execute();
				}
			}
		}
		
		if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
		{
		
		}
		//process event for IMGUI
		ImGui_ImplSDL3_ProcessEvent(&e);
	}

	for (const auto& [key, command] : m_KeyboardBindings)
	{
		if (m_KeyboardState[SDL_GetScancodeFromKey(key.first, NULL)] && key.second == ButtonState::Held) 
			command.get()->Execute();
	}
	return true;
}

void dae::InputManager::BindCommand(SDL_Keycode key, ButtonState buttonState, std::unique_ptr<Command> command)
{
	m_KeyboardBindings.insert({ std::make_pair(key, buttonState), std::move(command) });
}

void dae::InputManager::UnbindCommand(SDL_Keycode key, ButtonState buttonState)
{
	m_KeyboardBindings.erase(std::make_pair(key, buttonState));
}

