#pragma once
#include <Components/Component.h>
#include <IEventListener.h>
#include <EventManager.h>
#include "../TronEvents.h"

class PlayerScoreComponent final : public dae::Component, public dae::IEventListener
{
public:
	PlayerScoreComponent(dae::GameObject* owner) : Component(owner)
	{
		dae::EventManager::GetInstance().AddListener(this);
	}
	~PlayerScoreComponent() override
	{
		dae::EventManager::GetInstance().RemoveListener(this);
	}

	void OnEvent(const dae::Event& event) override
	{
		if (event.id == TronEventIds::EnemyDestroyed)
		{
			m_Score += event.value;
			SDL_Log("Player score updated: %d", m_Score);
		}
	}
	int GetScore() const { return m_Score; }

private:
	int m_Score{};
};