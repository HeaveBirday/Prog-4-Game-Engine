#include "BulletComponent.h"
#include "ObjectTypeComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "../TronEvents.h"

#include <SDL3/SDL_log.h>
#include <EventManager.h>
void dae::BulletComponent::Update(float )
{

}

void dae::BulletComponent::OnEvent(const Event& event)
{
	if (event.id != EventIds::Collision) return;

	// Check if the bullet is involved in the collision
	if (event.objectA != &GetOwner() && event.objectB != &GetOwner()) return;

	// Determine the other object involved in the collision
	GameObject* otherObject = 
		event.objectA == &GetOwner() ? event.objectB : event.objectA;
	auto bulletType = GetOwner().GetComponent<ObjectTypeComponent>();
	auto otherType = otherObject->GetComponent<ObjectTypeComponent>();
	if (!bulletType || !otherType) return;
	
	//Type Checking for what the bullet collided with
	if (bulletType->GetType() == ObjectType::PlayerBullet &&
		(otherType->GetType() == ObjectType::EnemyTank ||
		 otherType->GetType() == ObjectType::Recognizer))
	{
		EnemyHit(otherObject);
		return;
	}

	if(bulletType->GetType() == ObjectType::EnemyBullet &&
		otherType->GetType() == ObjectType::Player)
	{
		PlayerHit();
		return;
	}
}

void dae::BulletComponent::EnemyHit(GameObject* otherObject)
{
		//TODO Could do this in an event system if I have time over to polish this
		auto health = otherObject->GetComponent<HealthComponent>();
		if (health)
		{
			bool destroyed = health->TakeDamage(1);
			if (destroyed)
			{
				auto score = otherObject->GetComponent<ScoreComponent>();
				if (score)
				{
					EventManager::GetInstance().QueueEvent(
						dae::Event{
							TronEventIds::EnemyDestroyed,
							-1,
							score->GetScoreValue(),
							nullptr,
							nullptr
						});

					SDL_Log("Adding score: %d", score->GetScoreValue());

				}
			}


		}
	GetOwner().Destroy();
}

void dae::BulletComponent::PlayerHit()
{
	EventManager::GetInstance().QueueEvent(
		dae::Event{
			TronEventIds::PlayerHit,
			-1,
			1,
			nullptr,
			nullptr
		}
	);
	GetOwner().Destroy();
	return;
}
