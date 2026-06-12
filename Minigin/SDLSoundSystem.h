#pragma once
#include "SoundSystem.h"
#include <memory>
#include <string>

namespace dae
{
	// SDLSoundSystem is a concrete implementation of the SoundSystem interface using SDL_mixer for audio playback
	// It manages sound loading, playback, and muting functionality, and runs a dedicated thread for processing sound requests
	// The implementation details are hidden using the Pimpl idiom to reduce compilation dependencies and improve encapsulation
	// Note: The actual audio processing is done in a separate thread to avoid blocking the main game loop, and synchronization is handled using mutexes and condition variables
	class SDLSoundSystem final : public SoundSystem
	{
	public :
		SDLSoundSystem();
		~SDLSoundSystem() override;

		void Play(sound_id id, float vlolume) override;
		void PlayLooping(sound_id id, float volume) override;
		void LoadSound(sound_id id, const std::string& filePath) override;
		
		void SetMuted(bool muted) override;
		bool IsMuted() const override;
		void ToggleMuted() override;
		void StopLooping() override;
	private:

		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}