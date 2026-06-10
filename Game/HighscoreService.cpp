#include "HighscoreService.h"
#include <fstream>
#include <algorithm>
#include <SDL3/SDL_log.h>
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

bool HighscoreService::IsHighscore(int score) const
{
	if (m_Highscores.size() < 10)
		return true;

	return score > m_Highscores.back().score;
}

void HighscoreService::AddScore(const std::string& playerName, int score)
{

	if (playerName.size() != 3)
	{
		SDL_Log("Player name must be exactly 3 characters: %s", playerName.c_str());
		return;
	}

	m_Highscores.push_back({ playerName, score });

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