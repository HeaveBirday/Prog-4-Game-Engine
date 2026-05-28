#pragma once
#include <Event.h>

namespace TronEventIds
{
	constexpr dae::EventId EnemyDestroyed = dae::make_sdbm_hash("EnemyDestroyed");
	constexpr dae::EventId PlayerHit = dae::make_sdbm_hash("PlayerHit");
	constexpr dae::EventId ScoreChanged = dae::make_sdbm_hash("ScoreChanged");
	constexpr dae::EventId LevelComplete = dae::make_sdbm_hash("LevelComplete");
	constexpr dae::EventId ResetLevel = dae::make_sdbm_hash("ResetLevel");
	constexpr dae::EventId GameOver = dae::make_sdbm_hash("GameOver");

}