#include "GameOverState.h"

#include <SceneManager.h>
#include <Scene.h>
#include <GameObject.h>
#include <ResourceManager.h>
#include <Components/TextComponent.h>

#include "../GameSession.h"

#include <SDL3/SDL.h>
#include <string>

#include "GameStateManager.h"
#include "MainMenuState.h"
#include <InputManager.h>
extern GameStateManager g_GameStateManager;

void GameOverState::OnEnter()
{
	m_ScoreSaved = false;
	m_WasUpPressed = false;
	m_WasDownPressed = false;
	m_WasEnterPressed = false;

	auto& scene = dae::SceneManager::GetInstance().CreateScene();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 32);

	//Loading highscores and adding the current score if it qualifies
	m_HighscoreService.Load();
	m_PlayerName = "AAA";
	m_SelectedCharIndex = 0;
	m_IsEnteringName = true;

	//Setting the title of the game over screen
	auto title = std::make_unique<dae::GameObject>();
	title->SetPosition(400.f, 50.f);
	auto& titleText = title->AddComponent<dae::TextComponent>(font.get(), SDL_Color{ 255, 0, 0, 255 });
	titleText.SetText("GAME OVER");
	scene.Add(std::move(title));

	// Displaying the final score of the player
	auto scoreTextObj = std::make_unique<dae::GameObject>();
	scoreTextObj->SetPosition(20.f, 100.f);
	auto& scoreText = scoreTextObj->AddComponent<dae::TextComponent>(font.get(), SDL_Color{ 255,255,255,255 });

	scoreText.SetText("Final Score: " + std::to_string(GameSession::Score));
	scene.Add(std::move(scoreTextObj));

	auto nameObj = std::make_unique<dae::GameObject>();
	nameObj->SetPosition(20.f, 140.f);

	m_NameText = &nameObj->AddComponent<dae::TextComponent>(font.get(), SDL_Color{ 255,255,255,255 });
	UpdateNameText();

	scene.Add(std::move(nameObj));


	//Getting highscores out of the service and displaying them
	m_HighscoreEntries.clear();

	auto highscoreTitleObj = std::make_unique<dae::GameObject>();
	highscoreTitleObj->SetPosition(600.f, 100.f);
	auto& titleComp = highscoreTitleObj->AddComponent<dae::TextComponent>(font.get(),SDL_Color{ 255,255,255,255 });
	titleComp.SetText("HIGHSCORES:");
	scene.Add(std::move(highscoreTitleObj));

	float yPos{ 140.f };

	for (int i = 0; i < 10; ++i)
	{
		auto lineObj = std::make_unique<dae::GameObject>();
		lineObj->SetPosition(700.f, yPos);

		auto& lineText = lineObj->AddComponent<dae::TextComponent>(font.get(),SDL_Color{ 255,255,255,255 });

		m_HighscoreEntries.push_back(&lineText);

		scene.Add(std::move(lineObj));

		yPos += 40.f;
	}

	UpdateHighscoreText();

	scene.Update(0.f);
}

void GameOverState::OnExit()
{
	dae::SceneManager::GetInstance().RemoveAll();
	m_NameText = nullptr;
	m_ScoreSaved = false;
	m_HighscoreText = nullptr;
	m_HighscoreEntries.clear();
}

void GameOverState::HandleInput()
{
	const bool* keyboardState = SDL_GetKeyboardState(nullptr);
	auto& input = dae::InputManager::GetInstance();

	const bool isUpPressed = keyboardState[SDL_SCANCODE_UP] ||
		input.IsPressed(dae::ControllerInput::Button::DPadUp);

	const bool isDownPressed = keyboardState[SDL_SCANCODE_DOWN] ||
		input.IsPressed(dae::ControllerInput::Button::DPadDown);

	const bool isConfirmPressed = keyboardState[SDL_SCANCODE_RETURN] ||
		input.IsPressed(dae::ControllerInput::Button::A);

	const bool isBackPressed = keyboardState[SDL_SCANCODE_ESCAPE] ||
		input.IsPressed(dae::ControllerInput::Button::B);

	if (isUpPressed && !m_WasUpPressed)
		NextLetter();

	if (isDownPressed && !m_WasDownPressed)
		PreviousLetter();

	if (isConfirmPressed && !m_WasEnterPressed)
		ConfirmLetter();

	if (m_ScoreSaved && isBackPressed)
		g_GameStateManager.SetState(std::make_unique<MainMenuState>());

	m_WasUpPressed = isUpPressed;
	m_WasDownPressed = isDownPressed;
	m_WasEnterPressed = isConfirmPressed;
}

void GameOverState::Update(float )
{
}

void GameOverState::UpdateNameText()
{
	
	if (!m_NameText)
		return;

	m_NameText->SetText("Enter Name: " + m_PlayerName);

}

void GameOverState::UpdateHighscoreText()
{
	const auto& highscores = m_HighscoreService.GetHighscores();

	for (size_t i = 0; i <m_HighscoreEntries.size(); ++i)
	{
		if (!m_HighscoreEntries[i])
			continue;

		if (i < highscores.size())
		{
			const auto& entry = highscores[i];

			m_HighscoreEntries[i]->SetText(std::to_string(i + 1) + ". " +entry.playerName + " - " + std::to_string(entry.score));
		}
		else
		{
			m_HighscoreEntries[i]->SetText("");
		}
	}
}

void GameOverState::NextLetter()
{
	if (!m_IsEnteringName || m_ScoreSaved) return;

	char& currentChar = m_PlayerName[m_SelectedCharIndex];
	if (currentChar == 'Z')
	{
		currentChar = 'A';
	}
	else
	{
		currentChar++;
	}
	UpdateNameText();
}

void GameOverState::PreviousLetter()
{
	if (!m_IsEnteringName || m_ScoreSaved) return;

	char& currentChar = m_PlayerName[m_SelectedCharIndex];
	if (currentChar == 'A')
	{
		currentChar = 'Z';
	}
	else
	{
		currentChar--;
	}
	UpdateNameText();
}

void GameOverState::ConfirmLetter()
{
	if (!m_IsEnteringName || m_ScoreSaved) return;

	++m_SelectedCharIndex;

	if (m_SelectedCharIndex >= 3)
	{
		m_HighscoreService.AddScore(m_PlayerName, GameSession::Score);
		UpdateHighscoreText();
		m_ScoreSaved = true;
		m_IsEnteringName = false;

		if (m_NameText)
		{
			m_NameText->SetText("Score Saved ESC OR GAMEPAD B:MAIN MENU");
		}
		return;
	}
	UpdateNameText();
}
