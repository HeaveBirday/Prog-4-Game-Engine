#pragma once

struct GameSession
{
	static int Lives;
	static int Score;
	static int CurrentLevel;

	static void Reset()
	{
		Lives = 3;
		Score = 0;
		CurrentLevel = 1;
	}
};