#pragma once
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