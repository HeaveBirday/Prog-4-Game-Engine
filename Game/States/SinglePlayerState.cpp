#include "SinglePlayerState.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Components/TextComponent.h"

#include <SDL3/SDL.h>
#include "Components/RenderComponent.h"
#include "Components/HealthComponent.h"
#include "Components/VelocityComponent.h"
#include "Components/CollisionComponent.h"
#include "../Component/ShooterComponent.h"
#include "../Component/TankComponent.h"
#include "../Component/ObjectTypeComponent.h"
#include "../Component/HealthComponent.h"
#include "../Component/ScoreComponent.h"
#include "../Component/PlayerScoreComponent.h"
#include "../Component/LivesComponent.h"
#include "../Component/WallCollisionComponent.h"

#include "InputManager.h"
#include "../Commands/MoveCommand.h"
#include "../Commands/ShootBulletCommand.h"
#include "../Commands/RotateTurretCommand.h"
#include "../GameCollisionLayer.h"
#include <glm/glm.hpp>

#include "../LevelBuilder.h"

void SinglePlayerState::OnEnter()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();
	auto go = std::make_unique<dae::GameObject>();

	//Title Screen thingies
	go->SetPosition(292.f, 20.f);
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto& txtComponent = go->AddComponent<dae::TextComponent>(font.get(), SDL_Color{ 255, 255, 0, 255 });
	txtComponent.SetText("Single Player State Loaded");
	scene.Add(std::move(go));


	
	auto playerObjects = tron::CreatePlayer(scene, { 200.f,200.f });
	auto* greenTankPtr = playerObjects.player;
	auto* turretPtr = playerObjects.turret;

	auto& input = dae::InputManager::GetInstance();
	auto& velocityComponent = *greenTankPtr->GetComponent<dae::VelocityComponent>();
	auto& tankComponent = *greenTankPtr->GetComponent<dae::TankComponent>();

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
		std::make_unique<MoveCommand>(velocityComponent,tankComponent, glm::vec2{ -1,0 }));
	input.BindCommand(SDLK_A, dae::InputManager::ButtonState::Released,
		std::make_unique<MoveCommand>(velocityComponent,tankComponent, glm::vec2{ 0,0 }));

	input.BindCommand(SDLK_D, dae::InputManager::ButtonState::Held, 
		std::make_unique<MoveCommand>(velocityComponent,tankComponent, glm::vec2{ 1,0 }));
	input.BindCommand(SDLK_D, dae::InputManager::ButtonState::Released,
		std::make_unique<MoveCommand>(velocityComponent,tankComponent, glm::vec2{ 0,0 }));

	//Turret Commands
	input.BindCommand(SDLK_Z, dae::InputManager::ButtonState::Held,
		std::make_unique<RotateTurretCommand>(tankComponent, *turretPtr, -180.f));
	input.BindCommand(SDLK_X, dae::InputManager::ButtonState::Held,
		std::make_unique<RotateTurretCommand>(tankComponent, *turretPtr, 180.f));
	input.BindCommand(SDLK_SPACE, dae::InputManager::ButtonState::Held,
		std::make_unique<dae::ShootBulletCommand>(greenTankPtr));


	//Enemy Blue tank thingies for testing purposes
	tron::LoadLevel1(scene);

}

void SinglePlayerState::OnExit()
{
	dae::InputManager::GetInstance().ClearCommands();
	dae::SceneManager::GetInstance().RemoveAll();
}

void SinglePlayerState::HandleInput()
{
}

void SinglePlayerState::Update(float )
{
}
