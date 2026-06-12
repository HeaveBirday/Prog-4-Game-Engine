#pragma once
#include <Commands/Command.h>
#include "../States/GameStateManager.h"
// SkipLevelCommand is a simple command that tells the GameStateManager to skip to the next level when executed. 
// It is bound to a specific input in the SinglePlayerState for testing purposes, allowing developers to quickly skip levels during development and testing.
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