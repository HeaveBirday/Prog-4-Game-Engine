#pragma once

// Base class for all game states.
// Each state handles its own input, update logic, and scene setup/cleanup.
class GameState
{
public:

	virtual ~GameState() = default;
	virtual void OnEnter() {}
	virtual void OnExit() {}

	virtual void HandleInput() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void SkipLevel() {}
};