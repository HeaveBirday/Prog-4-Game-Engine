#include "GameOverState.h"
#include <SceneManager.h>
#include <Scene.h>
#include <GameObject.h>
#include <ResourceManager.h>
#include <Components/TextComponent.h>
#include "../GameSession.h"

#include <SDL3/SDL.h>
#include <string>


void GameOverState::OnEnter()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 32);
	auto title = std::make_unique<dae::GameObject>();
	title->SetPosition(300.f, 150.f);
	auto& titleText = title->AddComponent<dae::TextComponent>(font.get(), SDL_Color{ 255, 0, 0, 255 });
	titleText.SetText("GAME OVER");
	scene.Add(std::move(title));

	auto scoreTextObj = std::make_unique<dae::GameObject>();
	scoreTextObj->SetPosition(300.f, 220.f);
	auto& scoreText = scoreTextObj->AddComponent<dae::TextComponent>(font.get(), SDL_Color{ 255,255,255,255 });

	scoreText.SetText("Final Score: " + std::to_string(GameSession::Score));
	scene.Add(std::move(scoreTextObj));

	scene.Update(0.f);
}

void GameOverState::OnExit()
{
	dae::SceneManager::GetInstance().RemoveAll();

}

void GameOverState::HandleInput()
{
}

void GameOverState::Update(float )
{
}
