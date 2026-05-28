#pragma once
#include <Components/Component.h>
#include <IEventListener.h>
#include <EventManager.h>
#include "../TronEvents.h"
#include <SDL3/SDL_log.h>
#include "../GameSession.h"


class LivesComponent final : public dae::Component, public dae::IEventListener
{
public:
	LivesComponent(dae::GameObject* owner) : Component(owner)
	{
		dae::EventManager::GetInstance().AddListener(this);
	}
	~LivesComponent() override
	{
		dae::EventManager::GetInstance().RemoveListener(this);
	}

	void OnEvent(const dae::Event& event) override
	{
		if (event.id == TronEventIds::PlayerHit)
		{
			GameSession::Lives--;
			SDL_Log("Player hit! Lives remaining: %d", GameSession::Lives);

			if (GameSession::Lives <= 0)
			{
				SDL_Log("Player has no more lives! Game Over!");
				dae::EventManager::GetInstance().QueueEvent({
					TronEventIds::GameOver,
					-1,
					0,
					&GetOwner(),
					nullptr
					});
			}
			else
			{
				SDL_Log("Reset current level");

				dae::EventManager::GetInstance().QueueEvent({
					TronEventIds::ResetLevel,
					-1,
					0,
					&GetOwner(),
					nullptr
					});
			}
		}
	}

	int GetLives() const { return GameSession::Lives; }

private:
	
};