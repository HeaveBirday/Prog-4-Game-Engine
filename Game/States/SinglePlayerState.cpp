#include "SinglePlayerState.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Components/TextComponent.h"
#include <Commands/ToggleMuteCommand.h>

#include <SDL3/SDL.h>

#include "Components/VelocityComponent.h"
#include "../Component/TankComponent.h"
#include "../Component/PlayerScoreComponent.h"
#include "../Component/LivesComponent.h"
#include "InputManager.h"
#include "../Commands/MoveCommand.h"
#include "../Commands/ShootBulletCommand.h"
#include "../Commands/RotateTurretCommand.h"
#include <glm/glm.hpp>
#include "../LevelBuilder.h"

#include "GameStateManager.h"
#include "GameOverState.h"
#include "../Commands/SkipLevelCommand.h"

extern GameStateManager g_GameStateManager;

void SinglePlayerState::OnEnter()
{
	dae::EventManager::GetInstance().AddListener(this);

	LoadLevel();

}

void SinglePlayerState::OnExit()
{
	dae::EventManager::GetInstance().RemoveListener(this);
	dae::InputManager::GetInstance().ClearCommands();
	dae::SceneManager::GetInstance().RemoveAll();
}

void SinglePlayerState::HandleInput()
{
}

void SinglePlayerState::Update(float )
{
	if (m_ShouldResetLevel)
	{
		m_ShouldResetLevel = false;

		dae::InputManager::GetInstance().ClearCommands();
		dae::SceneManager::GetInstance().RemoveAll();

		LoadLevel();
	}

	if (m_ShouldGameOver)
	{
		m_ShouldGameOver = false;

		SDL_Log("Go to GameOverState later");
		g_GameStateManager.SetState(std::make_unique<GameOverState>());
	}
	if (m_ShouldLoadNextLevel)
	{
		m_ShouldLoadNextLevel = false;

		GameSession::CurrentLevel++;
		if (GameSession::CurrentLevel > 3) GameSession::CurrentLevel = 1;

		dae::InputManager::GetInstance().ClearCommands();
		dae::SceneManager::GetInstance().RemoveAll();

		LoadLevel();
	}
}

void SinglePlayerState::OnEvent(const dae::Event& event)
{
	if (event.id == TronEventIds::ResetLevel)
	{
		SDL_Log("SinglePlayerState: reset level ");
		m_ShouldResetLevel = true;
	}
	else if (event.id == TronEventIds::GameOver)
	{
		SDL_Log("SinglePlayerState: Game Over");
		m_ShouldGameOver = true;
	}
	else if (event.id == TronEventIds::EnemyDestroyed)
	{
		m_EnemiesAlive--;

		SDL_Log("Enemies alive: %d", m_EnemiesAlive);

		if (m_EnemiesAlive <= 0)
		{
			m_ShouldLoadNextLevel = true;
		}
	}
}

void SinglePlayerState::LoadLevel()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();
	auto go = std::make_unique<dae::GameObject>();

	//Title Screen thingies
	go->SetPosition(292.f, 20.f);
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto& txtComponent = go->AddComponent<dae::TextComponent>(font.get(), SDL_Color{ 255, 255, 0, 255 });
	txtComponent.SetText("Single Player State Loaded");
	scene.Add(std::move(go));

	// Game Manager to handle global game systems 
	auto gameManager = std::make_unique<dae::GameObject>();
	gameManager->AddComponent<LivesComponent>();
	gameManager->AddComponent<PlayerScoreComponent>();
	scene.Add(std::move(gameManager));

	//Creating Player with Turret Object and getting them from LevelBuilder as a struct
	const std::vector<std::string>* currentLevel = &tron::GetLevel1();

	switch (GameSession::CurrentLevel)
	{
	case 1:
		currentLevel = &tron::GetLevel1();
		break;
	case 2:
		currentLevel = &tron::GetLevel2();
		break;
	case 3:
		currentLevel = &tron::GetLevel3();
		break;
	}

	auto playerObjects = tron::CreatePlayer(scene, { 448.f, 192.f }, *currentLevel);
	BindPlayerInput(playerObjects.player, playerObjects.turret);

	switch (GameSession::CurrentLevel)
	{
	case 1:
		m_EnemiesAlive = tron::LoadLevel1(scene);
		break;
	case 2:
		m_EnemiesAlive = tron::LoadLevel2(scene);
		break;
	case 3:
		m_EnemiesAlive = tron::LoadLevel3(scene);
		break;
	}
	SDL_Log("Enemies alive: %d", m_EnemiesAlive);

}

void SinglePlayerState::BindPlayerInput(dae::GameObject* player, dae::GameObject* turretPtr)
{
	auto& input = dae::InputManager::GetInstance();
	auto& velocityComponent = *player->GetComponent<dae::VelocityComponent>();
	auto& tankComponent = *player->GetComponent<dae::TankComponent>();

	//Player Movement Commands
	input.BindCommand(SDLK_W, dae::InputManager::ButtonState::Held,
		std::make_unique<MoveCommand>(velocityComponent, tankComponent, glm::vec2{ 0,-1 }));
	input.BindCommand(SDLK_W, dae::InputManager::ButtonState::Released,
		std::make_unique<MoveCommand>(velocityComponent, tankComponent, glm::vec2{ 0,0 }));

	input.BindCommand(SDLK_S, dae::InputManager::ButtonState::Held,
		std::make_unique<MoveCommand>(velocityComponent, tankComponent, glm::vec2{ 0,1 }));
	input.BindCommand(SDLK_S, dae::InputManager::ButtonState::Released,
		std::make_unique<MoveCommand>(velocityComponent, tankComponent, glm::vec2{ 0,0 }));

	input.BindCommand(SDLK_A, dae::InputManager::ButtonState::Held,
		std::make_unique<MoveCommand>(velocityComponent, tankComponent, glm::vec2{ -1,0 }));
	input.BindCommand(SDLK_A, dae::InputManager::ButtonState::Released,
		std::make_unique<MoveCommand>(velocityComponent, tankComponent, glm::vec2{ 0,0 }));

	input.BindCommand(SDLK_D, dae::InputManager::ButtonState::Held,
		std::make_unique<MoveCommand>(velocityComponent, tankComponent, glm::vec2{ 1,0 }));
	input.BindCommand(SDLK_D, dae::InputManager::ButtonState::Released,
		std::make_unique<MoveCommand>(velocityComponent, tankComponent, glm::vec2{ 0,0 }));

	//Turret Commands
	input.BindCommand(SDLK_Z, dae::InputManager::ButtonState::Held,
		std::make_unique<RotateTurretCommand>(tankComponent, *turretPtr, -180.f));
	input.BindCommand(SDLK_X, dae::InputManager::ButtonState::Held,
		std::make_unique<RotateTurretCommand>(tankComponent, *turretPtr, 180.f));
	input.BindCommand(SDLK_SPACE, dae::InputManager::ButtonState::Held,
		std::make_unique<dae::ShootBulletCommand>(player));

	input.BindCommand(
		SDLK_F1,
		dae::InputManager::ButtonState::Released,
		std::make_unique<SkipLevelCommand>(g_GameStateManager));
	input.BindCommand(SDLK_F2,
		dae::InputManager::ButtonState::Released,
		std::make_unique<ToggleMuteCommand>());
}

void SinglePlayerState::SkipLevel()
{
	m_ShouldLoadNextLevel = true;
}
