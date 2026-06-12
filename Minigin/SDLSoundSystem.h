#pragma once
#include "SoundSystem.h"
#include <memory>
#include <string>

namespace dae
{
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