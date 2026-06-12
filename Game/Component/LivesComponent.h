#pragma once
#include <Components/Component.h>
#include <IEventListener.h>
#include <EventManager.h>
#include "../TronEvents.h"
#include <SDL3/SDL_log.h>
#include "../GameSession.h"
#include <ServiceLocator.h>
#include "../SoundIds.h"

// LivesComponent listens for PlayerHit events, reduces the player's lives, and triggers level reset or game over when necessary.
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
	// When a PlayerHit event is received, decrease lives and check for game over or level reset conditions
	void OnEvent(const dae::Event& event) override
	{
		if (event.id == TronEventIds::PlayerHit)
		{

			GameSession::Lives--;
			SDL_Log("Player hit! Lives remaining: %d", GameSession::Lives);

			if (GameSession::Lives <= 0)
			{
				SDL_Log("Player has no more lives! Game Over!");
				dae::ServiceLocator::GetSoundSystem().Play(dae::SoundIds::GameOver, 1.0f);
				//Game over event gets handled in the Gamemode State, which will trigger the transition to the GameOverState
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
				dae::ServiceLocator::GetSoundSystem().StopLooping();
				dae::ServiceLocator::GetSoundSystem().Play(dae::SoundIds::PlayerHit, 0.4f);
				dae::ServiceLocator::GetSoundSystem().PlayLooping(dae::SoundIds::BackgroundMusic, 0.2f);
				// Gets handled in the Gamemode State, which will trigger the reload of the current level
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