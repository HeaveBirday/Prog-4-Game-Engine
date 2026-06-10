#pragma once
#include "GameState.h"
#include "../HighscoreService.h"
#include <string>
#include <vector>

namespace dae
{
	class TextComponent;
}

class GameOverState final : public GameState
{
public:
	void OnEnter() override;
	void OnExit() override;

	void HandleInput() override;
	void Update(float dt) override;



private:
	void ConfirmLetter();
	void UpdateNameText();
	void PreviousLetter();
	void NextLetter();
	void UpdateHighscoreText();
	std::vector<dae::TextComponent*> m_HighscoreEntries;

	HighscoreService m_HighscoreService{ "Data/highscores.txt" };
	std::string m_PlayerName{"AAA"};
	bool m_ScoreSaved{ false };

	int m_SelectedCharIndex{ 0 };
	bool m_IsEnteringName{ true };	

	dae::TextComponent* m_NameText{};
	dae::TextComponent* m_HighscoreText{};

	bool m_WasUpPressed{ false };
	bool m_WasDownPressed{ false };
	bool m_WasEnterPressed{ false };
};