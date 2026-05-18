#pragma once
#include "GameState.h"

class VersusState final : public GameState
{
public:
	void OnEnter() override;
	void OnExit() override;

	void HandleInput() override;
	void Update(float deltaTime) override;
};