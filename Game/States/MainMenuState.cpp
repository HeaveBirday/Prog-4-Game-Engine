#include "MainMenuState.h"
#include "SinglePlayerState.h"
#include "GameStateManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Components/TextComponent.h"
#include "InputManager.h"
#include <SDL3/SDL.h>

extern GameStateManager g_GameStateManager;
void MainMenuState::OnEnter()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();
	auto go = std::make_unique<dae::GameObject>();
	go->SetPosition(300.f, 200.f);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto& text = go->AddComponent<dae::TextComponent>(font.get(), SDL_Color{ 255, 255, 0, 255 });
	text.SetText("Main Menu State Loaded, PRESS 1 FOR SINGLEPLAYER");
	scene.Add(std::move(go));
}

void MainMenuState::OnExit()
{
	dae::InputManager::GetInstance().ClearCommands();
	dae::SceneManager::GetInstance().RemoveAll();
}

void MainMenuState::HandleInput()
{
	const bool is1Pressed = SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_1];

	if(is1Pressed)
	{
		g_GameStateManager.SetState(std::make_unique<SinglePlayerState>());
	}
}

void MainMenuState::Update(float )
{
}
