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

#include "InputManager.h"
#include "../Commands/MoveCommand.h"
#include "../Commands/ShootBulletCommand.h"

#include <glm/glm.hpp>

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

	//Tank thingies
	auto greenTankTexture = dae::ResourceManager::GetInstance().LoadTexture("GreenTank.png");
	auto& input = dae::InputManager::GetInstance();
	const float keyboardSpeed = 120.f;
	auto greenTank = std::make_unique<dae::GameObject>();
	greenTank->SetPosition(200.f, 200.f);
	greenTank->AddComponent<dae::RenderComponent>(greenTankTexture);
	greenTank->AddComponent<dae::HealthComponent>(0, 3);

	auto& velocityComponent = greenTank->AddComponent<dae::VelocityComponent>(keyboardSpeed);
	greenTank->AddComponent<dae::CollisionComponent>(glm::vec2{ 32.f, 32.f });

	auto& tankComponent = greenTank->AddComponent<dae::TankComponent>(greenTankTexture->GetSize());
	auto* greenTankPtr = greenTank.get();
	greenTank->AddComponent<dae::ShooterComponent>();
	scene.Add(std::move(greenTank));

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

	input.BindCommand(SDLK_SPACE, dae::InputManager::ButtonState::Pressed,
		std::make_unique<dae::ShootBulletCommand>(greenTankPtr));

	

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
