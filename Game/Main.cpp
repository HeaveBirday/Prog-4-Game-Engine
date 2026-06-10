#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "ServiceLocator.h"
#include "SDLSoundSystem.h"
#include "SoundIds.h"
#include "States/GameStateManager.h"
#include "States/MainMenuState.h"


#include <filesystem>

namespace fs = std::filesystem;

GameStateManager g_GameStateManager;

static void load()
{
	dae::ServiceLocator::RegisterSoundSystem(std::make_unique<dae::SDLSoundSystem>());
	auto& soundSystem = dae::ServiceLocator::GetSoundSystem();
	soundSystem.LoadSound(dae::SoundIds::GunShot, dae::ResourceManager::GetInstance().GetFullPath("GunShot.wav"));
	//soundSystem.LoadSound(dae::SoundIds::BackgroundMusic, dae::ResourceManager::GetInstance().GetFullPath("BackgroundMusic.wav"));
	g_GameStateManager.SetState(std::make_unique<MainMenuState>());

	soundSystem.PlayLooping(dae::SoundIds::GunShot, 0.1f);
}

int main(int, char* [])
{
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if (!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load, [](float deltaTime)
		{
			g_GameStateManager.HandleInput();
			g_GameStateManager.Update(deltaTime);
		});
	return 0;
}
