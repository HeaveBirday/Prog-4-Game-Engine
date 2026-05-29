#include <SDL3/SDL.h>
#include "SDLSoundSystem.h"
#include <SDL3/SDL_log.h>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3/SDL_audio.h>
#include <memory>
namespace dae
{
	struct SoundRequest
	{
		sound_id id;
		float volume;
	};

	class SDLSoundSystem::Impl
	{

		struct MixerDeleter
		{
			void operator()(MIX_Mixer* mixer) const
			{
				if (mixer)
				{
					MIX_DestroyMixer(mixer);
					SDL_Log("Mixer device closed");
				}
			}
		};
		struct TrackDeleter
		{
			void operator()(MIX_Track* track) const
			{
				if (track)
				{
					MIX_DestroyTrack(track);
				}
			}
		};
		struct AudioDeleter
		{
			void operator()(MIX_Audio* audio) const
			{
				if (audio)
				{
					MIX_DestroyAudio(audio);
				}
			}
		};

		using MixerPtr = std::unique_ptr<MIX_Mixer, MixerDeleter>;
		using TrackPtr = std::unique_ptr<MIX_Track, TrackDeleter>;
		using AudioPtr = std::unique_ptr<MIX_Audio, AudioDeleter>;
	public:
		Impl()
		{
			if (!SDL_InitSubSystem(SDL_INIT_AUDIO))
			{
				SDL_Log("SDL_InitSubSystem failed: %s", SDL_GetError());
				return;
			}
			 else
			{
				SDL_Log("SDL audio subsystem initialized");
			}
			if (!MIX_Init())
			{
				SDL_Log("MIX_Init failed: %s", SDL_GetError());
			}
			else
			{
				SDL_Log("SDL_mixer initialized");
			}

			m_pMixer.reset(MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr));
			if (!m_pMixer)
			{
				SDL_Log("Failed to create mixer device: %s", SDL_GetError());
			}
			else
			{
				SDL_Log("Mixer device created successfully");
			}
			m_pTrack.reset(MIX_CreateTrack(m_pMixer.get()));
			if (!m_pTrack)
			{
				SDL_Log("Failed to create track: %s", SDL_GetError());
			}
			else
			{
				SDL_Log("Track created successfully");
			}
			m_SoundThread = std::thread(&Impl::ThreadMain, this);
		}
		~Impl()
		{
			{
				std::lock_guard lock(m_Mutex);
				m_IsRunning = false;

				while (!m_SoundQueue.empty())
				{
					m_SoundQueue.pop();
				}
			}

			m_ConditionVariable.notify_one();

			if (m_SoundThread.joinable())
			{
				m_SoundThread.join();
			}

			if (m_pTrack)
			{
				MIX_StopTrack(m_pTrack.get(), 0);
				MIX_SetTrackAudio(m_pTrack.get(), nullptr);
			}

			m_Sounds.clear();
			m_pTrack.reset();
			MIX_DestroyMixer(m_pMixer.release());
			m_pMixer.reset();
			MIX_Quit();
			SDL_QuitSubSystem(SDL_INIT_AUDIO);
			SDL_Log("SDLSoundSystem shutdown complete");
		}
		void Play(sound_id id, float volume)
		{
			{
				std::lock_guard lock(m_Mutex);
				if (!m_IsRunning) return;
				if (m_IsMuted) return;
				m_SoundQueue.push({ id, volume });
			}
			m_ConditionVariable.notify_one();
		}

		void PlaySoundInternal(const SoundRequest& request)
		{
			auto it = m_Sounds.find(request.id);

			if (it == m_Sounds.end())
			{
				return;
			}

			if (!m_pTrack)
			{
				SDL_Log("No track available to play sound with id: %d", request.id);
				return;
			}

			if (!MIX_SetTrackAudio(m_pTrack.get(), it->second.get()))
			{
				SDL_Log("Failed to set track audio for sound id %d: %s", request.id, SDL_GetError());
				return;
			}
			MIX_SetTrackGain(m_pTrack.get(), request.volume);

			if (!MIX_PlayTrack(m_pTrack.get(), 0))
			{
				SDL_Log("Failed to play sound id %d: %s", request.id, SDL_GetError());
				return;
			}

		}
		void LoadSound(sound_id id, const std::string& filepath)
		{
			if (!std::filesystem::exists(filepath))
			{
				SDL_Log("Sound File Not Found: %s", filepath.c_str());
				return;
			}
			MIX_Audio* audio = MIX_LoadAudio(m_pMixer.get(), filepath.c_str(), false);
			if (!audio)
			{
				SDL_Log("Failed to load sound: %s, error: %s", filepath.c_str(), SDL_GetError());
				return;
			}
			m_Sounds[id] = AudioPtr{ audio };
			SDL_Log("Loaded sound id %d from file: %s", id, filepath.c_str());


		}
		void SetMuted(bool muted)
		{
			std::lock_guard lock(m_Mutex);
			m_IsMuted = muted;
		}

		bool IsMuted() const
		{
			return m_IsMuted;
		}

		void ToggleMuted()
		{
			std::lock_guard lock(m_Mutex);
			m_IsMuted = !m_IsMuted;

			SDL_Log("Sound muted: %s", m_IsMuted ? "true" : "false");
		}
		void ThreadMain()
		{
			while (true)
			{
				SoundRequest request{};

				{
					std::unique_lock lock(m_Mutex);

					m_ConditionVariable.wait(lock, [this]
						{
							return !m_SoundQueue.empty() || !m_IsRunning;
						});

					if (!m_IsRunning)
						return;

					request = m_SoundQueue.front();
					m_SoundQueue.pop();
				}

				PlaySoundInternal(request);
			}
		}
	private:
		std::queue<SoundRequest> m_SoundQueue; 
		std::mutex m_Mutex;
		std::thread m_SoundThread;
		bool m_IsRunning{ true };
		std::condition_variable m_ConditionVariable;
		std::unordered_map<sound_id, AudioPtr> m_Sounds;
		MixerPtr m_pMixer{};
		TrackPtr m_pTrack{};
		
		bool m_IsMuted{};
	};

	SDLSoundSystem::SDLSoundSystem() : m_pImpl{ std::make_unique<Impl>() }
	{

	}

	void SDLSoundSystem::SetMuted(bool muted)
	{
		m_pImpl->SetMuted(muted);
	}

	bool SDLSoundSystem::IsMuted() const
	{
		return m_pImpl->IsMuted();
	}

	void SDLSoundSystem::ToggleMuted()
	{
		m_pImpl->ToggleMuted();
	}

	SDLSoundSystem::~SDLSoundSystem() = default;

	void SDLSoundSystem::Play(sound_id id, float volume)
	{
		m_pImpl->Play(id, volume);
	}
	void SDLSoundSystem::LoadSound(sound_id id, const std::string& filePath)
	{
		m_pImpl->LoadSound(id, filePath);
	}
}
