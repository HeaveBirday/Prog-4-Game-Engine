#pragma once
#include "GameState.h"
#include <IEventListener.h>
#include <GameObject.h>
#include <Components/TextComponent.h>
#include <memory>
class SinglePlayerState final : public GameState, public dae::IEventListener
{
public:
	void OnEnter() override;
	void OnExit() override;

	void HandleInput() override;
	void Update(float deltaTime) override;

	void OnEvent(const dae::Event& event) override;
private:
	

	void UpdateHud();
	void LoadLevel();
	void BindPlayerInput(dae::GameObject* player, dae::GameObject* turretPtr);
	void SkipLevel() override;
	int m_EnemiesAlive{};

	bool m_ShouldResetLevel{};
	bool m_ShouldGameOver{};
	bool m_ShouldLoadNextLevel{};

	dae::TextComponent* m_ScoreText{};
	dae::TextComponent* m_LivesText{};
	std::shared_ptr<dae::Font> m_HudFont{};
};