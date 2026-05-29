#pragma once
#include <Commands/Command.h>
#include "../States/GameStateManager.h"

class SkipLevelCommand final : public Command
{
public:
	explicit SkipLevelCommand(GameStateManager& gameStateManager) : 
		m_GameStateManager(gameStateManager)
	{}

	void Execute(float) override
	{
		m_GameStateManager.SkipLevel();
	}
private:
	GameStateManager& m_GameStateManager;
};