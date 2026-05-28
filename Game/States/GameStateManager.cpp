#include "GameStateManager.h"
#include "GameState.h"
void GameStateManager::SetState(std::unique_ptr<GameState> newState)
{
	m_PendingState = std::move(newState);

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
	ProcessPendingStates();
}

void GameStateManager::ProcessPendingStates()
{
	if (!m_PendingState) return;

	if (m_CurrentState)
	{
		m_CurrentState->OnExit();
	}

	m_CurrentState = std::move(m_PendingState);

	if (m_CurrentState)
	{
		m_CurrentState->OnEnter();
	}
}

