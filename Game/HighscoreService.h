#pragma once
#include <string>
#include <vector>

struct HighscoreEntry
{
	std::string playerName;
	int score{};
};
// HighscoreService manages the loading, saving, and updating of high scores for the game. 
// It reads and writes high scores to a file, checks if a given score qualifies as a high score, and allows adding new high score entries.
class HighscoreService final
{
public:
	explicit HighscoreService(const std::string& filePath) 
		: m_FilePath(filePath) 
	{
	}

	void Load();
	void Save() const;

	bool IsHighscore(int score) const;
	void AddScore(const std::string& playerName, int score);

	const std::vector<HighscoreEntry>& GetHighscores() const { return m_Highscores; }
private:
	std::string m_FilePath;
	std::vector<HighscoreEntry> m_Highscores;
};

