#pragma once
#include <string>
#include <functional>
#include <filesystem>

namespace dae
{
	// The Minigin class is the core of the game engine, responsible for initializing subsystems, managing the main game loop,
	// and handling frame updates. It provides a simple interface for running the game by accepting load and update callbacks,
	// allowing for flexible game state management. The class also ensures proper cleanup of resources upon destruction.
	class Minigin final
	{
		// Static member to hold the SDL window instance, ensuring that it is shared across all instances of Minigin
		bool m_quit{};
		// m_Lag is used to accumulate the time difference between frames, allowing for fixed timestep updates in the game loop
		float m_Lag{};
		// m_FixedTimeStep defines the fixed time step for updates, ensuring consistent game logic updates regardless of frame rate variations
		const float m_FixedTimeStep{ 1.0f / 60.0f };

		std::function<void(float)> m_UpdateCallback;
	public:
		explicit Minigin(const std::filesystem::path& dataPath);
		~Minigin();
		void Run(const std::function<void()>& load, const std::function<void(float)>& update);
		void RunOneFrame();

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;
	};
}