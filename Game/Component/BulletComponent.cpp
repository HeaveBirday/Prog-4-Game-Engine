#include "BulletComponent.h"
#include "ObjectTypeComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "../TronEvents.h"


#include <cmath>
#include <SDL3/SDL_log.h>
#include <EventManager.h>
void dae::BulletComponent::Update(float)
{
	m_HasBouncedThisFrame = false;

	if (!m_Transform) return;
	m_PreviousPos = m_Transform->GetWorldPosition();
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
	if (otherType->GetType() == ObjectType::Wall)
	{
		if (!m_HasBouncedThisFrame)
		{
			Bounce(otherObject);
			m_HasBouncedThisFrame = true;
		}
		return;
	}

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

void dae::BulletComponent::Bounce(GameObject* wall)
{
	++m_BounceCount;

	if (m_BounceCount >= m_MaxBounces)
	{
		GetOwner().Destroy();
		return;
	}
	if (!m_Transform || !m_Collision || !m_Velocity) return;
	// Getting wall data for calculation on which side of the wall the bullet collided with
	auto* wallTransform = wall->GetTransform();
	auto* wallCollision = wall->GetComponent<CollisionComponent>();
	if (!wallTransform || !wallCollision) return;
	const glm::vec2 bulletSize = m_Collision->GetSize();
	const glm::vec2 wallSize = wallCollision->GetSize();

	const glm::vec2 currentPos = m_Transform->GetWorldPosition();
	const glm::vec2 wallPos = wallTransform->GetWorldPosition();

	const glm::vec2 bulletCenter = currentPos + bulletSize / 2.f;
	const glm::vec2 wallCenter = wallTransform->GetWorldPosition() + wallSize / 2.f;

	// Calculating overlap data
	const glm::vec2 difference = bulletCenter - wallCenter;
	const glm::vec2 overlap{
		(wallSize.x / 2.f + bulletSize.x / 2.f) - std::abs(difference.x),
		(wallSize.y / 2.f + bulletSize.y / 2.f) - std::abs(difference.y)
	};

	float projectionX{ glm::dot(glm::normalize(difference), glm::vec2{1,0}) };
	float projectionY{ glm::dot(glm::normalize(difference), glm::vec2{0,1}) };
	glm::vec2 wallNormal{};


	if (std::abs(projectionX) > std::abs(projectionY))
	{
		wallNormal = glm::vec2{ projectionX / std::abs(projectionX), 0 };
	}
	else
	{
		wallNormal = glm::vec2{ 0,projectionY / std::abs(projectionY)};
	}
	
	// Applying changes to direction
	m_Direction = glm::normalize(
	m_Direction - 2.f * glm::dot(m_Direction, wallNormal) * wallNormal);
	m_Velocity->SetDirection(m_Direction);
	m_Transform->SetWorldPosition(currentPos + wallNormal * std::abs(glm::dot(overlap, wallNormal)));
	
}
