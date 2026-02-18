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

#include "TransformComponent.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	{
		auto go = std::make_unique<dae::GameObject>();
		go->AddComponent<dae::TransformComponent>().SetPosition(0.f, 0.f);
		auto tex = dae::ResourceManager::GetInstance().LoadTexture("background.png");
		go->AddComponent<dae::RenderComponent>(tex);
		scene.Add(std::move(go));
	}
	
	{
		auto go = std::make_unique<dae::GameObject>();
		go->AddComponent<dae::TransformComponent>().SetPosition(358.f, 180.f);
		auto tex = dae::ResourceManager::GetInstance().LoadTexture("logo.png");
		go->AddComponent<dae::RenderComponent>(tex);
		scene.Add(std::move(go));
	}
	
	{
		auto go = std::make_unique<dae::GameObject>();
		go->AddComponent<dae::TransformComponent>().SetPosition(292.f, 20.f);
		auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
		auto& text = go->AddComponent<dae::TextComponent>(font, SDL_Color{ 255, 255, 0, 255 });
		text.SetText("Programming 4 Assignment");
		scene.Add(std::move(go));
	}
	//FPS Counter
	{
		auto go = std::make_unique<dae::GameObject>();
		go->AddComponent<dae::TransformComponent>().SetPosition(10.f, 10.f);
		auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
		auto& text = go->AddComponent<dae::TextComponent>(font, SDL_Color{ 255, 255, 255, 255 });
		text.SetText("0 FPS");
		go->AddComponent<dae::FPSComponent>(0.01f);
		scene.Add(std::move(go));
	}
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
