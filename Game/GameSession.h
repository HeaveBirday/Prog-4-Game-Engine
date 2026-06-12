#pragma once

// GameSession struct manages the player's current game state, including lives, score, and current level.
// It provides a Reset function to initialize these values at the start of a new game session or when the player loses all lives.
// This struct is used throughout the game to track and update the player's progress and status as they play through the levels.
struct GameSession
{
	static int Lives;
	static int Score;
	static int CurrentLevel;

	static void Reset()
	{
		Lives = 4;
		Score = 0;
		CurrentLevel = 1;
	}
};