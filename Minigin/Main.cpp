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
	
	//Logo
	
		go = std::make_unique<dae::GameObject>();
		go->SetPosition(358.f, 180.f);
		auto text = dae::ResourceManager::GetInstance().LoadTexture("logo.png");
		go->AddComponent<dae::RenderComponent>(text);
		go->AddComponent<dae::WiggleComponent>(80.f);
		scene.Add(std::move(go));
	
	//Title text
	
		go = std::make_unique<dae::GameObject>();
		go->SetPosition(292.f, 20.f);
		auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
		auto& txtComponent = go->AddComponent<dae::TextComponent>(font.get(), SDL_Color{255, 255, 0, 255});
		txtComponent.SetText("Programming 4 Assignment");
		scene.Add(std::move(go));
	
	//FPS Counter
	
		go = std::make_unique<dae::GameObject>();
		go->SetPosition(10.f, 10.f);
		auto fpsFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
		auto& fpsText = go->AddComponent<dae::TextComponent>(fpsFont.get(), SDL_Color{255, 255, 255, 255});
		fpsText.SetText("0 FPS");
		go->AddComponent<dae::FPSComponent>(0.01f);
		scene.Add(std::move(go));
	
	//Ball Excercise Pivots SceneGraph Child/Parent

		// pivot at center of screen
		auto pivot = std::make_unique<dae::GameObject>();
		pivot->SetPosition(400.f, 300.f); // adjust to your window center
		auto* pivotPtr = pivot.get();
		scene.Add(std::move(pivot));

		// ball A (child of pivot)
		auto ballTex = dae::ResourceManager::GetInstance().LoadTexture("ball.png");

		auto ballA = std::make_unique<dae::GameObject>();
		ballA->SetParent(pivotPtr, true);
		ballA->AddComponent<dae::RenderComponent>(ballTex);
		ballA->AddComponent<dae::RotatorComponent>(120.f, 2.0f); // radius, speed
		auto* ballAPtr = ballA.get();
		scene.Add(std::move(ballA));

		// ball B (child of ball A)
		auto ballB = std::make_unique<dae::GameObject>();
		ballB->SetParent(ballAPtr, true);
		ballB->AddComponent<dae::RenderComponent>(ballTex);
		ballB->AddComponent<dae::RotatorComponent>(60.f, -4.0f); // opposite direction
		scene.Add(std::move(ballB));
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
