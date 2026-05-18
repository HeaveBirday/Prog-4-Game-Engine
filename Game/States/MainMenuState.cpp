#include "MainMenuState.h"
#include "GameStateManager.h"
#include "SinglePlayerState.h"
#include "CoopState.h"
#include "VersusState.h"
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
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	
	{
		auto go = std::make_unique<dae::GameObject>();
		go->SetPosition(300.f, 150.f);
		auto& text = go->AddComponent<dae::TextComponent>(font.get(), SDL_Color{ 255, 255, 0, 255 });
		text.SetText("Main Menu State Loaded");
		scene.Add(std::move(go));
	}
	
	{
		auto go = std::make_unique<dae::GameObject>();
		go->SetPosition(300.f, 200.f);
		auto& singlePlayerTxt = go->AddComponent<dae::TextComponent>(font.get(), SDL_Color{ 255, 255, 0, 255 });
		singlePlayerTxt.SetText("PRESS 1 FOR SINGLEPLAYER MODE");
		scene.Add(std::move(go));
	}
	{
		auto go = std::make_unique<dae::GameObject>();
		go->SetPosition(300.f, 250.f);
		auto& coopPlayerTxt = go->AddComponent<dae::TextComponent>(font.get(), SDL_Color{ 255, 255, 0, 255 });
		coopPlayerTxt.SetText("PRESS 2 FOR CO-OP MODE");
		scene.Add(std::move(go));
	}

	{
		auto go = std::make_unique<dae::GameObject>();
		go->SetPosition(300.f, 300.f);
		auto& versusModeTxt = go->AddComponent<dae::TextComponent>(font.get(), SDL_Color{ 255, 255, 0, 255 });
		versusModeTxt.SetText("PRESS 3 FOR VERSUS MODE");
		scene.Add(std::move(go));
	}
	

	
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
	
	const bool is2Pressed = SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_2];
	if (is2Pressed)
	{
		g_GameStateManager.SetState(std::make_unique<CoopState>());
	}
	const bool is3Pressed = SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_3];
	if (is3Pressed)
	{
		g_GameStateManager.SetState(std::make_unique<VersusState>());
	}


}

void MainMenuState::Update(float )
{
}
