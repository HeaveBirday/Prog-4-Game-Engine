#include <stdexcept>
#include <sstream>
#include <iostream>

#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif

#include <SDL3/SDL.h>
//#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include <Renderer.h>
#include "ResourceManager.h"
#include "EventManager.h"
#include <chrono>
#include <filesystem>
#include "ServiceLocator.h"

SDL_Window* g_window{};

void LogSDLVersion(const std::string& message, int major, int minor, int patch)
{
#if WIN32
	std::stringstream ss;
	ss << message << major << "." << minor << "." << patch << "\n";
	OutputDebugString(ss.str().c_str());
#else
	std::cout << message << major << "." << minor << "." << patch << "\n";
#endif
}

#ifdef __EMSCRIPTEN__
#include "emscripten.h"

void LoopCallback(void* arg)
{
	static_cast<dae::Minigin*>(arg)->RunOneFrame();
}
#endif

void PrintSDLVersion()
{
	LogSDLVersion("Compiled with SDL", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);
	int version = SDL_GetVersion();
	LogSDLVersion("Linked with SDL ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
	LogSDLVersion("Compiled with SDL_ttf ",	SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION,SDL_TTF_MICRO_VERSION);
	version = TTF_Version();
	LogSDLVersion("Linked with SDL_ttf ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version),	SDL_VERSIONNUM_MICRO(version));
}

// The Minigin constructor initializes the SDL video subsystem, creates the main application window,
// and initializes the Renderer, InputManager, and ResourceManager with the provided data path.
dae::Minigin::Minigin(const std::filesystem::path& dataPath)
{
	PrintSDLVersion();
	
	if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
	{
		SDL_Log("Renderer error: %s", SDL_GetError());
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Tron BattleTanks",
		1024,
		576,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);
	InputManager::GetInstance().Init();
	ResourceManager::GetInstance().Init(dataPath);
}
// The Minigin destructor cleans up resources by destroying the sound system, renderer, and SDL window, and then quits SDL.
dae::Minigin::~Minigin()
{
	dae::ServiceLocator::DestroySoundSystem();
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}
// The Run method takes two callbacks: one for loading resources and one for updating the game state.
void dae::Minigin::Run(const std::function<void()>& load, const std::function<void(float)>& update)
{
	load();

	m_UpdateCallback = update;
#ifndef __EMSCRIPTEN__
	// The main game loop runs continuously until the m_quit flag is set to true, calling RunOneFrame() on each iteration to process input, update the game state, and render the scene.
	while (!m_quit)
		RunOneFrame();
#else
	emscripten_set_main_loop_arg(&LoopCallback, this, 0, true);
#endif
}
// The RunOneFrame method processes a single frame of the game loop, including input handling,
// updating the game state with fixed time steps, processing events, and rendering the scene.
// It also manages frame timing to maintain a consistent frame rate.
void dae::Minigin::RunOneFrame()
{
	using clock = std::chrono::high_resolution_clock;
	const auto frameStart = clock::now();

	static auto lastTime = clock::now();
	const auto currentTime = clock::now();
	float dt = std::chrono::duration<float>(currentTime - lastTime).count();
	lastTime = currentTime;

	if (dt > 0.25f) dt = 0.25f;
	m_quit = !InputManager::GetInstance().ProcessInput(dt);

	if (m_UpdateCallback)
	{
		m_UpdateCallback(dt);
	}

	m_Lag += dt;
	while (m_Lag >= m_FixedTimeStep)
	{
		SceneManager::GetInstance().FixedUpdate(m_FixedTimeStep);

		m_Lag -= m_FixedTimeStep;
	}	
	SceneManager::GetInstance().Update(dt);

	dae::EventManager::GetInstance().ProcessEvents();

	Renderer::GetInstance().Render();


	const auto frameEnd = clock::now();
	float frameTime = std::chrono::duration<float>(frameEnd - frameStart).count();

	if (frameTime < m_FixedTimeStep)
	{
		Uint32 delayMs = static_cast<Uint32>((m_FixedTimeStep - frameTime) * 1000.0f);
		if (delayMs > 0)
			SDL_Delay(delayMs);
	}
}
