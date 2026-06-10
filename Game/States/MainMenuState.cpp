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
#include "../GameSession.h"
extern GameStateManager g_GameStateManager;
void MainMenuState::OnEnter()
{
	GameSession::Reset();
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
		singlePlayerTxt.SetText("PRESS 1 OR GAMEPAD A FOR SINGLEPLAYER MODE");
		scene.Add(std::move(go));
	}
	{
		auto go = std::make_unique<dae::GameObject>();
		go->SetPosition(300.f, 250.f);
		auto& coopPlayerTxt = go->AddComponent<dae::TextComponent>(font.get(), SDL_Color{ 255, 255, 0, 255 });
		coopPlayerTxt.SetText("PRESS 2 OR GAMEPAD B FOR CO-OP MODE");
		scene.Add(std::move(go));
	}

	{
		auto go = std::make_unique<dae::GameObject>();
		go->SetPosition(300.f, 300.f);
		auto& versusModeTxt = go->AddComponent<dae::TextComponent>(font.get(), SDL_Color{ 255, 255, 0, 255 });
		versusModeTxt.SetText("PRESS 3 OR GAMEPAD X FOR VERSUS MODE");
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
	auto& input = dae::InputManager::GetInstance();
	const bool* keyboardState = SDL_GetKeyboardState(nullptr);

	const bool singleStateSelected = keyboardState[SDL_SCANCODE_1] ||
		input.IsPressed(dae::ControllerInput::Button::A);

	const bool coopStateSelected = keyboardState[SDL_SCANCODE_2] ||
		input.IsPressed(dae::ControllerInput::Button::B);

	const bool versusStateSelected = keyboardState[SDL_SCANCODE_3] ||
		input.IsPressed(dae::ControllerInput::Button::X);



	if(singleStateSelected)
	{
		g_GameStateManager.SetState(std::make_unique<SinglePlayerState>());
	}
	
	if (coopStateSelected)
	{
		g_GameStateManager.SetState(std::make_unique<CoopState>());
	}
	if (versusStateSelected)
	{
		g_GameStateManager.SetState(std::make_unique<VersusState>());
	}


}

void MainMenuState::Update(float )
{
}
