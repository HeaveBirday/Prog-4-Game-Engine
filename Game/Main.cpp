#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
//#include "TextObject.h" not needed anymore since we are using TextComponent now
#include "Scene.h"

#include "Components/TransformComponent.h"
#include "Components/RenderComponent.h"
#include "Components/TextComponent.h"
#include "Components/FPSComponent.h"
#include "Components/WiggleComponent.h"
#include "Components/RotatorComponent.h"
#include "Components/HealthComponent.h"

#include "InputManager.h"
#include "ControllerInput.h"
#include "Commands/PrintCommand.h"
#include "Commands/MoveCommand.h"
#include "Commands/LoseLifeCommand.h"
#include "Components/LivesDisplayComponent.h"

#include "EventManager.h"
#include "DebugEventListener.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();
	//Background

	auto go = std::make_unique<dae::GameObject>();
	go->SetPosition(0.f, 0.f);
	auto tex = dae::ResourceManager::GetInstance().LoadTexture("background.png");
	go->AddComponent<dae::RenderComponent>(tex);
	scene.Add(std::move(go));

	////Logo
	//
	//	go = std::make_unique<dae::GameObject>();
	//	go->SetPosition(358.f, 180.f);
	//	auto text = dae::ResourceManager::GetInstance().LoadTexture("logo.png");
	//	go->AddComponent<dae::RenderComponent>(text);
	//	go->AddComponent<dae::WiggleComponent>(80.f);
	//	scene.Add(std::move(go));
	//
	////Title text
	//
		go = std::make_unique<dae::GameObject>();
		go->SetPosition(292.f, 20.f);
		auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		auto& txtComponent = go->AddComponent<dae::TextComponent>(font.get(), SDL_Color{255, 255, 0, 255});
		txtComponent.SetText("Space to attack green tank, K to attack blue tank");
		scene.Add(std::move(go));
	//
	////FPS Counter
	//
	//	go = std::make_unique<dae::GameObject>();
	//	go->SetPosition(10.f, 10.f);
	//	auto fpsFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	//	auto& fpsText = go->AddComponent<dae::TextComponent>(fpsFont.get(), SDL_Color{255, 255, 255, 255});
	//	fpsText.SetText("0 FPS");
	//	go->AddComponent<dae::FPSComponent>(0.01f);
	//	scene.Add(std::move(go));
	//
	////Ball Excercise Pivots SceneGraph Child/Parent

	//	// pivot at center of screen
	//	auto pivot = std::make_unique<dae::GameObject>();
	//	pivot->SetPosition(400.f, 300.f); // adjust to your window center
	//	auto* pivotPtr = pivot.get();
	//	scene.Add(std::move(pivot));

	//	// ball A (child of pivot)
	//	auto ballTex = dae::ResourceManager::GetInstance().LoadTexture("ball.png");

	//	auto ballA = std::make_unique<dae::GameObject>();
	//	ballA->SetParent(pivotPtr, true);
	//	ballA->AddComponent<dae::RenderComponent>(ballTex);
	//	ballA->AddComponent<dae::RotatorComponent>(120.f, 2.0f); // radius, speed
	//	auto* ballAPtr = ballA.get();
	//	scene.Add(std::move(ballA));

	//	// ball B (child of ball A)
	//	auto ballB = std::make_unique<dae::GameObject>();
	//	ballB->SetParent(ballAPtr, true);
	//	ballB->AddComponent<dae::RenderComponent>(ballTex);
	//	ballB->AddComponent<dae::RotatorComponent>(60.f, -4.0f); // opposite direction
	//	scene.Add(std::move(ballB));


	dae::InputManager::GetInstance().BindCommand(
		dae::ControllerInput::Button::A,
		dae::InputManager::ButtonState::Pressed,
		std::make_unique<PrintCommand>()
	);

	auto greenTankTexture = dae::ResourceManager::GetInstance().LoadTexture("GreenTank.png");
	auto blueTankTexture = dae::ResourceManager::GetInstance().LoadTexture("BlueTank.png");

	//Green tank stuff thingies
	go = std::make_unique<dae::GameObject>();
	go->SetPosition(200.f, 200.f);
	go->AddComponent<dae::RenderComponent>(greenTankTexture);
	go->AddComponent<dae::HealthComponent>(0, 3);
	auto* greenTankPtr = go.get();
	scene.Add(std::move(go));

	//Blue tank stuff thingies
	go = std::make_unique<dae::GameObject>();
	go->SetPosition(400.f, 200.f);
	go->AddComponent<dae::RenderComponent>(blueTankTexture);
	go->AddComponent<dae::HealthComponent>(1, 3);
	auto* blueTankPtr = go.get();
	scene.Add(std::move(go));

	{
		const float controllerSpeed = 4.f;
		const float keyboardSpeed = 2.f;
		auto& input = dae::InputManager::GetInstance();

		input.BindCommand(SDLK_W,
			dae::InputManager::ButtonState::Held,
			std::make_unique<MoveCommand>(greenTankPtr, MoveCommand::Direction::Up, keyboardSpeed));

		input.BindCommand(SDLK_S,
			dae::InputManager::ButtonState::Held,
			std::make_unique<MoveCommand>(greenTankPtr, MoveCommand::Direction::Down, keyboardSpeed));

		input.BindCommand(SDLK_A,
			dae::InputManager::ButtonState::Held,
			std::make_unique<MoveCommand>(greenTankPtr, MoveCommand::Direction::Left, keyboardSpeed));

		input.BindCommand(SDLK_D,
			dae::InputManager::ButtonState::Held,
			std::make_unique<MoveCommand>(greenTankPtr, MoveCommand::Direction::Right, keyboardSpeed));


		//Lose life command for green tank
		input.BindCommand(SDLK_SPACE,
			dae::InputManager::ButtonState::Pressed,
			std::make_unique<LoseLifeCommand>(greenTankPtr));

		//Controller bindings for blue tank
		input.BindCommand(dae::ControllerInput::Button::DPadUp,
			dae::InputManager::ButtonState::Held,
			std::make_unique<MoveCommand>(blueTankPtr, MoveCommand::Direction::Up, controllerSpeed));

		input.BindCommand(dae::ControllerInput::Button::DPadDown,
			dae::InputManager::ButtonState::Held,
			std::make_unique<MoveCommand>(blueTankPtr, MoveCommand::Direction::Down, controllerSpeed));

		input.BindCommand(dae::ControllerInput::Button::DPadLeft,
			dae::InputManager::ButtonState::Held,
			std::make_unique<MoveCommand>(blueTankPtr, MoveCommand::Direction::Left, controllerSpeed));

		input.BindCommand(dae::ControllerInput::Button::DPadRight,
			dae::InputManager::ButtonState::Held,
			std::make_unique<MoveCommand>(blueTankPtr, MoveCommand::Direction::Right, controllerSpeed));

		//Lose life command for blue tank
		input.BindCommand(SDLK_K,
			dae::InputManager::ButtonState::Pressed,
			std::make_unique<LoseLifeCommand>(blueTankPtr));
	}
	

	{
		auto livesUi = std::make_unique<dae::GameObject>();
		livesUi->SetPosition(20.f, 50.f);

		auto uiFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);

		auto& livesText = livesUi->AddComponent<dae::TextComponent>(
			uiFont.get(),
			SDL_Color{ 255, 255, 255, 255 }

		);

		livesText.SetText("Lives: 3");
		livesUi->AddComponent<dae::LivesDisplayComponent>(0);

		scene.Add(std::move(livesUi));

	}

	{
		auto blueLivesUi = std::make_unique<dae::GameObject>();
		blueLivesUi->SetPosition(20.f, 80.f);

		auto uiFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);

		auto& blueLivesText = blueLivesUi->AddComponent<dae::TextComponent>(
			uiFont.get(),
			SDL_Color{ 255, 255, 255, 255 }
		);

		blueLivesText.SetText("Lives P2: 3");
		blueLivesUi->AddComponent<dae::LivesDisplayComponent>(1);

		scene.Add(std::move(blueLivesUi));
	}

	dae::DebugEventListener debugListener{};
	dae::EventManager::GetInstance().AddListener(&debugListener);

	dae::EventManager::GetInstance().QueueEvent({ dae::EventType::PlayerDied, 0, 0 });
	dae::EventManager::GetInstance().ProcessEvents();

	dae::EventManager::GetInstance().RemoveListener(&debugListener);
}

int main(int, char*[]) 
{
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}
