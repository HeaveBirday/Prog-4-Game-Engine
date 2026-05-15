#pragma once
#include <Event.h>
#include <SoundSystem.h>

namespace dae
{
	namespace SoundIds
	{
		constexpr sound_id GunShot = make_sdbm_hash("GunShot");
		constexpr sound_id Explosion = make_sdbm_hash("Explosion");
		constexpr sound_id BackgroundMusic = make_sdbm_hash("BackgroundMusic");
	}
}