#pragma once
#include <memory>

class GameState;
class GameStateManager final
{
public: 
	void SetState(std::unique_ptr<GameState> newState);

	void HandleInput();
	void Update(float deltaTime);
	void SkipLevel();

private:
	void ProcessPendingStates();
	std::unique_ptr<GameState> m_CurrentState{};
	std::unique_ptr<GameState> m_PendingState{};
};