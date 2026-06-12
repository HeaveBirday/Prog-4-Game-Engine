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
		constexpr sound_id GameOver = make_sdbm_hash("GameOver");
		constexpr sound_id PlayerHit = make_sdbm_hash("PlayerHit");
		constexpr sound_id EnemyHit = make_sdbm_hash("EnemyHit");
		constexpr sound_id NewLevel = make_sdbm_hash("NewLevel");


	}
}