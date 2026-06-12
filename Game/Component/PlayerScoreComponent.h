#pragma once
#include <Components/Component.h>
#include <IEventListener.h>
#include <EventManager.h>
#include "../TronEvents.h"
#include "../GameSession.h"
// PlayerScoreComponent listens for EnemyDestroyed events and updates the player's score accordingly. It also provides a method to retrieve the current score for display in the HUD.
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
			GameSession::Score += event.value;
			SDL_Log("Player score updated: %d", GameSession::Score);
		}
	}
	int GetScore() const { return GameSession::Score; }

private:

};