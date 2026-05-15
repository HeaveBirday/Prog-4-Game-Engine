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
    };
}