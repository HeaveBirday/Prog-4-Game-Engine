#pragma once
#include "GameState.h"

class SinglePlayerState final : public GameState
{
public:
	void OnEnter() override;
	void OnExit() override;

	void HandleInput() override;
	void Update(float deltaTime) override;
};