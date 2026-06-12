#include "HighscoreService.h"
#include <fstream>
#include <algorithm>
#include <SDL3/SDL_log.h>

// The Load function reads high scores from a file, validates them, and keeps only the top 10 scores.
void HighscoreService::Load()
{
	m_Highscores.clear();
	std::ifstream file(m_FilePath);
	if(!file.is_open())
	{
		SDL_Log("Failed to open highscore file: %s", m_FilePath.c_str());
		return;
	}
	std::string name;
	int score{};
	// Read each line from the file, expecting a player name and score. Validate the name length and log any invalid entries.
	while (file >> name >> score)
	{
		if (name.size() == 3)
		{
			m_Highscores.push_back({ name, score });

		}
		else
		{
			SDL_Log("Invalid entry in highscore file: %s %d", name.c_str(), score);
		}
	}
	// Sort the high scores in descending order and keep only the top 10 entries.
	std::sort(m_Highscores.begin(), m_Highscores.end(),
		[](const HighscoreEntry& a, const HighscoreEntry& b)
		{
			return a.score > b.score;
		});

	if (m_Highscores.size() > 10)
	{
		m_Highscores.resize(10);
	}
}
// The Save function writes the current high scores to a file, logging any errors that occur during the file opening process.
void HighscoreService::Save() const
{
	std::ofstream file(m_FilePath);
	if (!file.is_open())
	{
		SDL_Log("Failed to open highscore file for writing: %s", m_FilePath.c_str());
		return;
	}
	for (const auto& entry : m_Highscores)
	{
		file << entry.playerName << " " << entry.score << "\n";
	}
}
// IsHighscore checks if a given score qualifies as a high score by comparing it to the current list of high scores,
// allowing for new entries if there are fewer than 10 scores or if the new score is higher than the lowest existing score.
bool HighscoreService::IsHighscore(int score) const
{
	if (m_Highscores.size() < 10)
		return true;

	return score > m_Highscores.back().score;
}
// AddScore adds a new score to the high score list if the player's name is valid (exactly 3 characters),
void HighscoreService::AddScore(const std::string& playerName, int score)
{

	if (playerName.size() != 3)
	{
		SDL_Log("Player name must be exactly 3 characters: %s", playerName.c_str());
		return;
	}

	m_Highscores.push_back({ playerName, score });
	// After adding the new score, sort the high scores in descending order and keep only the top 10 entries.
	std::sort(m_Highscores.begin(), m_Highscores.end(),
		[](const HighscoreEntry& a, const HighscoreEntry& b)
		{
			return a.score > b.score;
		});

	if (m_Highscores.size() > 10)
	{
		m_Highscores.resize(10);
	}

	Save();
}