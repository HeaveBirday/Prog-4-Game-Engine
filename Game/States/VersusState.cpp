#include "VersusState.h"
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

#include <ServiceLocator.h>
#include "../SoundIds.h"
extern GameStateManager g_GameStateManager;

void VersusState::OnEnter()
{
	dae::EventManager::GetInstance().AddListener(this);

	LoadLevel();
}

void VersusState::OnExit()
{
	dae::EventManager::GetInstance().RemoveListener(this);
	dae::InputManager::GetInstance().ClearCommands();
	m_ScoreText = nullptr;
	m_LivesText = nullptr;

	dae::SceneManager::GetInstance().RemoveAll();

	m_HudFont.reset();
}

void VersusState::HandleInput()
{
}

void VersusState::Update(float )
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
		return;
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
	UpdateHud();
}

void VersusState::OnEvent(const dae::Event& event)
{
	if (event.id == TronEventIds::ResetLevel)
	{
		m_ShouldResetLevel = true;
	}
	else if (event.id == TronEventIds::GameOver)
	{
		m_ShouldGameOver = true;
	}
	else if (event.id == TronEventIds::EnemyDestroyed)
	{
		// Check if the enemy tank is the EnemyPlayer. If yes the Input Bindings get cleared, and the Original Player's bindings get rebinded
		if (event.objectA == m_VersusPlayer || event.objectB == m_VersusPlayer)
		{
			m_VersusPlayer = nullptr;
			m_VersusPlayerTurret = nullptr;
			dae::InputManager::GetInstance().ClearCommands();

			if (m_PlayerOne && m_PlayerOneTurret)
				BindPlayerOneInput(m_PlayerOne, m_PlayerOneTurret);

			return;
		}
		m_EnemiesAlive--;

		SDL_Log("Enemies alive: %d", m_EnemiesAlive);

		if (m_EnemiesAlive <= 0)
		{
			dae::ServiceLocator::GetSoundSystem().Play(dae::SoundIds::NewLevel, 1.0f);
			m_ShouldLoadNextLevel = true;
		}
	}
}

void VersusState::UpdateHud()
{
	if (m_ScoreText)
		m_ScoreText->SetText("Score: " + std::to_string(GameSession::Score));

	if (m_LivesText)
		m_LivesText->SetText("Lives: " + std::to_string(GameSession::Lives));
}
// The same as COOP state taking a second player and binding it to the controller, but the SecondPlayer acts as an enemy. Once the EnemyPlayer is killed, his input gets reset 
void VersusState::LoadLevel()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

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
	//Player One Initialization
	glm::vec2 playerOneSpawn = tron::GetPlayerStartPos(*currentLevel);
	auto playerOneObjects = tron::CreatePlayer(scene, playerOneSpawn, *currentLevel);
	m_PlayerOne = playerOneObjects.player;
	m_PlayerOneTurret = playerOneObjects.turret;
	BindPlayerOneInput(m_PlayerOne, m_PlayerOneTurret);

	//Player Two Initialization
	glm::vec2 VersusPlayerSpawn = tron::GetVersusPlayerStartPos(*currentLevel);
	auto VersusPlayerObjects = tron::CreateVersusPlayer(scene, VersusPlayerSpawn);
	m_VersusPlayerTurret = VersusPlayerObjects.turret;
	m_VersusPlayer = VersusPlayerObjects.player;
	BindVersusPlayer(m_VersusPlayer, m_VersusPlayerTurret);

	//Enemies Initialization
	m_EnemiesAlive = tron::LoadLevel(scene, *currentLevel);
	SDL_Log("Enemies alive: %d", m_EnemiesAlive);

	//HUD INFO
	m_HudFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	auto scoreObj = std::make_unique<dae::GameObject>();
	scoreObj->SetPosition(20.f, 20.f);
	m_ScoreText = &scoreObj->AddComponent<dae::TextComponent>(m_HudFont.get(), SDL_Color{ 255, 255, 255, 255 });
	scene.Add(std::move(scoreObj));

	auto livesObj = std::make_unique<dae::GameObject>();
	livesObj->SetPosition(180.f, 20.f);
	m_LivesText = &livesObj->AddComponent<dae::TextComponent>(m_HudFont.get(), SDL_Color{ 255, 255, 255, 255 });
	scene.Add(std::move(livesObj));

	UpdateHud();
}

void VersusState::BindPlayerOneInput(dae::GameObject* player, dae::GameObject* turretPtr)
{
	auto& input = dae::InputManager::GetInstance();
	auto& velocityComponent = *player->GetComponent<dae::VelocityComponent>();
	auto& tankComponent = *player->GetComponent<dae::TankComponent>();

	//Player Movement Commands

	//UP
	input.BindCommand(SDLK_W, dae::InputManager::ButtonState::Held,
		std::make_unique<MoveCommand>(velocityComponent, tankComponent, glm::vec2{ 0,-1 }));
	input.BindCommand(SDLK_W, dae::InputManager::ButtonState::Released,
		std::make_unique<MoveCommand>(velocityComponent, tankComponent, glm::vec2{ 0,0 }));

	//DOWN
	input.BindCommand(SDLK_S, dae::InputManager::ButtonState::Held,
		std::make_unique<MoveCommand>(velocityComponent, tankComponent, glm::vec2{ 0,1 }));
	input.BindCommand(SDLK_S, dae::InputManager::ButtonState::Released,
		std::make_unique<MoveCommand>(velocityComponent, tankComponent, glm::vec2{ 0,0 }));

	//LEFT
	input.BindCommand(SDLK_A, dae::InputManager::ButtonState::Held,
		std::make_unique<MoveCommand>(velocityComponent, tankComponent, glm::vec2{ -1,0 }));
	input.BindCommand(SDLK_A, dae::InputManager::ButtonState::Released,
		std::make_unique<MoveCommand>(velocityComponent, tankComponent, glm::vec2{ 0,0 }));

	//RIGHT
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

void VersusState::BindVersusPlayer(dae::GameObject* player, dae::GameObject* turretPtr)
{
	auto& input = dae::InputManager::GetInstance();
	auto& velocityComponent = *player->GetComponent<dae::VelocityComponent>();
	auto& tankComponent = *player->GetComponent<dae::TankComponent>();

	//Player Movement Commands

	//UP
	input.BindCommand(dae::ControllerInput::Button::DPadUp, dae::InputManager::ButtonState::Held,
		std::make_unique<MoveCommand>(velocityComponent, tankComponent, glm::vec2{ 0,-1 }));
	input.BindCommand(dae::ControllerInput::Button::DPadUp, dae::InputManager::ButtonState::Released,
		std::make_unique<MoveCommand>(velocityComponent, tankComponent, glm::vec2{ 0,0 }));

	//DOWN
	input.BindCommand(dae::ControllerInput::Button::DPadDown, dae::InputManager::ButtonState::Held,
		std::make_unique<MoveCommand>(velocityComponent, tankComponent, glm::vec2{ 0,1 }));
	input.BindCommand(dae::ControllerInput::Button::DPadDown, dae::InputManager::ButtonState::Released,
		std::make_unique<MoveCommand>(velocityComponent, tankComponent, glm::vec2{ 0,0 }));


	//LEFT
	input.BindCommand(dae::ControllerInput::Button::DPadLeft, dae::InputManager::ButtonState::Held,
		std::make_unique<MoveCommand>(velocityComponent, tankComponent, glm::vec2{ -1,0 }));
	input.BindCommand(dae::ControllerInput::Button::DPadLeft, dae::InputManager::ButtonState::Released,
		std::make_unique<MoveCommand>(velocityComponent, tankComponent, glm::vec2{ 0,0 }));

	//RIGHT
	input.BindCommand(dae::ControllerInput::Button::DPadRight, dae::InputManager::ButtonState::Held,
		std::make_unique<MoveCommand>(velocityComponent, tankComponent, glm::vec2{ 1,0 }));
	input.BindCommand(dae::ControllerInput::Button::DPadRight, dae::InputManager::ButtonState::Released,
		std::make_unique<MoveCommand>(velocityComponent, tankComponent, glm::vec2{ 0,0 }));

	//Turret Commands
	input.BindCommand(dae::ControllerInput::Button::X, dae::InputManager::ButtonState::Held,
		std::make_unique<RotateTurretCommand>(tankComponent, *turretPtr, -180.f));
	input.BindCommand(dae::ControllerInput::Button::B, dae::InputManager::ButtonState::Held,
		std::make_unique<RotateTurretCommand>(tankComponent, *turretPtr, 180.f));
	input.BindCommand(dae::ControllerInput::Button::A, dae::InputManager::ButtonState::Held,
		std::make_unique<dae::ShootBulletCommand>(player));
}

void VersusState::SkipLevel()
{
	m_ShouldLoadNextLevel = true;
}
