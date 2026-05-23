#pragma once
#include <Components/Component.h>
#include <IEventListener.h>
#include <EventManager.h>
#include "../TronEvents.h"
#include <SDL3/SDL_log.h>

class LivesComponent final : public dae::Component, public dae::IEventListener
{
public:
	LivesComponent(dae::GameObject* owner, int lives) : Component(owner), m_Lives(lives)
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
			m_Lives--;
			SDL_Log("Player hit! Lives remaining: %d", m_Lives);

			if(m_Lives <=0)
			{
				SDL_Log("Player has no more lives! Game Over!");
				// Here could trigger a game over event or handle it as needed
			}
			else
			{
				SDL_Log("Reset current level");
			}
		}
	}

	int GetLives() const { return m_Lives; }

private:
	int m_Lives{};
};