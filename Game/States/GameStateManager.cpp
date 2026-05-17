#include "GameStateManager.h"
#include "GameState.h"
void GameStateManager::SetState(std::unique_ptr<GameState> newState)
{
	if (m_CurrentState)
	{
		m_CurrentState->OnExit();
	}

	m_CurrentState = std::move(newState);
	if (m_CurrentState)
	{
		m_CurrentState->OnEnter();
	}
}

void GameStateManager::HandleInput()
{
	if (m_CurrentState)
	{
		m_CurrentState->HandleInput();
	}
}
void GameStateManager::Update(float deltaTime)
{
	if (m_CurrentState)
	{
		m_CurrentState->Update(deltaTime);
	}
}

