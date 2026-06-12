#include <SDL3/SDL.h>
#include "SDLSoundSystem.h"
#include <SDL3/SDL_log.h>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3/SDL_audio.h>
#include <memory>
#include <vector>
namespace dae
{
	struct SoundRequest
	{
		sound_id id;
		float volume;
	};
	// SDLSoundSystem::Impl is the internal implementation class for SDLSoundSystem, following the Pimpl idiom to hide implementation details
	class SDLSoundSystem::Impl
	{
		//SDL_mixer gives me raw pointers and requires specific destroy functions.
		//I wrapped them in unique_ptr with custom deleters so resource cleanup happens automatically(RAII)
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
		// Constructor initializes the SDL audio subsystem, SDL_mixer, creates a mixer device, and sets up sound tracks for playback
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
			constexpr int amountOfSfxTracks{ 8 };
			// Create multiple tracks for simultaneous SFX playback
			for (int idx{}; idx < amountOfSfxTracks; ++idx)
			{
				TrackPtr track{ MIX_CreateTrack(m_pMixer.get()) };

				if (!track)
				{
					SDL_Log("Failed to create SFX track: %s", SDL_GetError());
					continue;
				}

				m_pSfxTracks.push_back(std::move(track));
			}
			// Create a dedicated track for looping music playback
			m_pLoopingTrack.reset(MIX_CreateTrack(m_pMixer.get()));
			if (!m_pLoopingTrack)
			{
				SDL_Log("Failed to create music track: %s", SDL_GetError());
			}
			else
			{
				SDL_Log("Music track created successfully");
			}
			m_SoundThread = std::thread(&Impl::ThreadMain, this);
		}
		// Destructor ensures proper cleanup of resources, stops the sound thread, and shuts down SDL audio and SDL_mixer subsystems
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

			for (auto& track : m_pSfxTracks)
			{
				if (track)
				{
					MIX_StopTrack(track.get(), 0);
					MIX_SetTrackAudio(track.get(), nullptr);
				}
			}

			if (m_pLoopingTrack)
			{
				MIX_StopTrack(m_pLoopingTrack.get(), 0);
				MIX_SetTrackAudio(m_pLoopingTrack.get(), nullptr);
			}

			m_Sounds.clear();
			m_pSfxTracks.clear();
			m_pLoopingTrack.reset();
			MIX_DestroyMixer(m_pMixer.release());
			m_pMixer.reset();
			MIX_Quit();
			SDL_QuitSubSystem(SDL_INIT_AUDIO);
			SDL_Log("SDLSoundSystem shutdown complete");
		}
		MIX_Track* FindFreeSfxTrack()
		{
			for (auto& track : m_pSfxTracks)
			{
				if (!MIX_TrackPlaying(track.get()))
				{
					return track.get();
				}
			}

			return nullptr;
		}
		// Play method queues a sound request for playback, ensuring thread-safe access to the sound queue and checking for mute state before adding requests
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
		// PlayLooping method sets up a sound to play in a loop on the dedicated looping track, checking for the existence of the sound and the looping track, and applying the specified volume
		void PlayLooping(sound_id id, float volume)
		{
			auto it = m_Sounds.find(id);

			if (it == m_Sounds.end())
			{
				SDL_Log("Sound with id %d not found for looping playback", id);
				return;
			}
			if (!m_pLoopingTrack)
			{
				SDL_Log("No looping track available");
				return;
			}
			if (m_IsMuted) return;

			if (!MIX_SetTrackAudio(m_pLoopingTrack.get(), it->second.get()))
			{
				SDL_Log("Failed to set looping track audio for sound id %d: %s", id, SDL_GetError());
				return;
			}
			MIX_SetTrackGain(m_pLoopingTrack.get(), volume);

			SDL_PropertiesID loopProperties = SDL_CreateProperties();
			SDL_SetNumberProperty(loopProperties, MIX_PROP_PLAY_LOOPS_NUMBER, -1);

			if (!MIX_PlayTrack(m_pLoopingTrack.get(), loopProperties))
			{
				SDL_Log("Failed to play looping sound id %d: %s", id, SDL_GetError());
				return;
			}
			m_CurrentLoopingSound = id;
			m_CurrentLoopingVolume = volume;
			m_HasLoopingSound = true;
			SDL_DestroyProperties(loopProperties);

		}
		// StopLooping method stops any currently playing looping sound on the looping track and resets the state to indicate that there is no active looping sound
		void StopLooping()
		{
			if (m_pLoopingTrack)
			{
				MIX_StopTrack(m_pLoopingTrack.get(), 0);
				MIX_SetTrackAudio(m_pLoopingTrack.get(), nullptr);
			}

			m_HasLoopingSound = false;
		}
		// PlaySoundInternal is the internal method that processes a sound request by finding the corresponding audio data, 
		// selecting a free SFX track, and initiating playback with the specified volume, 
		// while also handling error cases such as missing sounds or unavailable tracks
		void PlaySoundInternal(const SoundRequest& request)
		{
			auto it = m_Sounds.find(request.id);

			if (it == m_Sounds.end())
			{
				return;
			}

			MIX_Track* track = FindFreeSfxTrack();

			if (!track)
			{
				SDL_Log("No free SFX track available");
				return;
			}

			if (!MIX_SetTrackAudio(track, it->second.get()))
			{
				SDL_Log("Failed to set track audio for sound id %d: %s", request.id, SDL_GetError());
				return;
			}

			MIX_SetTrackGain(track, request.volume);

			if (!MIX_PlayTrack(track, 0))
			{
				SDL_Log("Failed to play sound id %d: %s", request.id, SDL_GetError());
			}

		}
		// LoadSound method loads an audio file into the sound system, associating it with a sound ID,
		// and logs the outcome of the loading process, including error handling for missing files or loading failures
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
		// SetMuted, IsMuted, and ToggleMuted methods manage the mute state of the sound system,
		// allowing for thread-safe toggling of mute status and ensuring that currently playing sounds are stopped when muting
		void SetMuted(bool muted)
		{
			std::lock_guard lock(m_Mutex);
			m_IsMuted = muted;
		}

		bool IsMuted() const
		{
			return m_IsMuted;
		}
		// ToggleMuted method toggles the mute state and handles stopping or resuming playback of looping sounds based on the new mute state,
		// ensuring that the appropriate actions are taken to reflect the change in mute status
		void ToggleMuted()
		{
			std::lock_guard lock(m_Mutex);
			m_IsMuted = !m_IsMuted;

			if (m_IsMuted)
			{
				if (m_pLoopingTrack)
				{
					MIX_StopTrack(m_pLoopingTrack.get(), 0);
				}
			}
			else
			{
				if (m_HasLoopingSound)
				{
					auto it = m_Sounds.find(m_CurrentLoopingSound);
					if (it != m_Sounds.end() && m_pLoopingTrack)
					{
						MIX_SetTrackAudio(m_pLoopingTrack.get(), it->second.get());
						MIX_SetTrackGain(m_pLoopingTrack.get(), m_CurrentLoopingVolume);

						SDL_PropertiesID props = SDL_CreateProperties();
						SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
						MIX_PlayTrack(m_pLoopingTrack.get(), props);
						SDL_DestroyProperties(props);
					}
				}
			}
			SDL_Log("Sound muted: %s", m_IsMuted ? "true" : "false");
		}
		// ThreadMain is the main loop for the sound processing thread, which waits for sound requests to be added to the queue and processes them sequentially,
		// ensuring that sound playback is handled in a separate thread to avoid blocking the main game loop
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
		std::vector<TrackPtr> m_pSfxTracks;
		TrackPtr m_pLoopingTrack{};
		bool m_IsMuted{};
		sound_id m_CurrentLoopingSound{};
		float m_CurrentLoopingVolume{};
		bool m_HasLoopingSound{ false };
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

	void SDLSoundSystem::StopLooping()
	{
		m_pImpl->StopLooping();
	}

	SDLSoundSystem::~SDLSoundSystem() = default;

	void SDLSoundSystem::Play(sound_id id, float volume)
	{
		m_pImpl->Play(id, volume);
	}
	void SDLSoundSystem::PlayLooping(sound_id id, float volume)
	{
		m_pImpl->PlayLooping(id, volume);
	}
	void SDLSoundSystem::LoadSound(sound_id id, const std::string& filePath)
	{
		m_pImpl->LoadSound(id, filePath);
	}
}
