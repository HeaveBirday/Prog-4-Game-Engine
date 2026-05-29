#pragma once
#include "GameState.h"
#include <IEventListener.h>
#include <GameObject.h>

class SinglePlayerState final : public GameState, public dae::IEventListener
{
public:
	void OnEnter() override;
	void OnExit() override;

	void HandleInput() override;
	void Update(float deltaTime) override;

	void OnEvent(const dae::Event& event) override;
private:
	void LoadLevel();
	void BindPlayerInput(dae::GameObject* player, dae::GameObject* turret);
	void SkipLevel() override;
	int m_EnemiesAlive{};

	bool m_ShouldResetLevel{};
	bool m_ShouldGameOver{};
	bool m_ShouldLoadNextLevel{};
};