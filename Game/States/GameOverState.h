#pragma once
#include "GameState.h"
#include "../HighscoreService.h"
#include <string>
#include <vector>
#include <memory>
#include <Font.h>
namespace dae
{
	class TextComponent;
}

// The GameOverState class represents the state of the game when the player has lost all their lives. 
// It handles displaying the game over screen, allowing the player to enter their name for the high score list,
// and managing the high score entries. It inherits from GameState and implements the necessary functions for entering, exiting, handling input, and updating the state.
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

	bool m_WasUpPressed{ false };
	bool m_WasDownPressed{ false };
	bool m_WasEnterPressed{ false };
	std::shared_ptr<dae::Font> m_Font{};
};