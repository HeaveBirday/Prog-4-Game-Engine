#pragma once
#include <Event.h>

namespace TronEventIds
{
	constexpr dae::EventId EnemyDestroyed = dae::make_sdbm_hash("EnemyDestroyed");
	constexpr dae::EventId PlayerHit = dae::make_sdbm_hash("PlayerHit");
	constexpr dae::EventId ScoreChanged = dae::make_sdbm_hash("ScoreChanged");
	constexpr dae::EventId GameOver = dae::make_sdbm_hash("GameOver");
}