#pragma once
#include <memory>


// Owns the active GameState and safely switches to a new state.
// State changes are delayed until Update() so states are not replaced
// in the middle of their own input handling.
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