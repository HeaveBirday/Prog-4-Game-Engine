#pragma once
#include "SoundSystem.h"
#include <string>
namespace dae
{
    class NullSoundSystem final : public SoundSystem
    {
    public:
        void Play(sound_id, float) override {}
        void LoadSound(sound_id, const std::string&) override
        {
        }
		void PlayLooping(sound_id, float) override {}
        void SetMuted(bool) override {}
        bool IsMuted() const override { return true; }
        void ToggleMuted() override {}
    };
}