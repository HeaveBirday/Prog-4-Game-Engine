#pragma once
#include "GameState.h"

class GameOverState final : public GameState
{
public:
	void OnEnter() override;
	void OnExit() override;

	void HandleInput() override;
	void Update(float dt) override;

};