#include "BulletComponent.h"
#include "ObjectTypeComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "../TronEvents.h"


#include <cmath>
#include <SDL3/SDL_log.h>
#include <EventManager.h>
// BulletComponent handles the behavior of bullets, including movement, collision responses (bouncing, damaging enemies, hitting the player), and lifetime management.
void dae::BulletComponent::Update(float dt)
{
	m_HasBouncedThisFrame = false;

	if (!m_Transform) return;
	m_PreviousPos = m_Transform->GetWorldPosition();

	m_LifeTime -= dt;
	if (m_LifeTime <= 0.f)
	{
		GetOwner().Destroy();
		return;
	}
}
// OnEvent listens for collision events and determines if the bullet is involved. 
// If it is, it checks the type of the other object and responds accordingly (bouncing off walls, damaging enemies, hitting the player).
void dae::BulletComponent::OnEvent(const Event& event)
{
	if (GetOwner().IsDestroyed()) return;
	if (event.id != EventIds::Collision) return;

	// Check if the bullet is involved in the collision
	if (event.objectA != &GetOwner() && event.objectB != &GetOwner()) return;

	// Determine the other object involved in the collision
	GameObject* otherObject = 
		event.objectA == &GetOwner() ? event.objectB : event.objectA;
	auto bulletType = GetOwner().GetComponent<ObjectTypeComponent>();
	auto otherType = otherObject->GetComponent<ObjectTypeComponent>();
	if (!bulletType || !otherType) return;
	
	SDL_Log(
		"Bullet collision: bullet type %d with other type %d",
		static_cast<int>(bulletType->GetType()),
		static_cast<int>(otherType->GetType())
	);
	//in case Bullet collides with another Bullet
	if (
		(bulletType->GetType() == ObjectType::PlayerBullet ||
			bulletType->GetType() == ObjectType::EnemyBullet)
		&&
		(otherType->GetType() == ObjectType::PlayerBullet ||
			otherType->GetType() == ObjectType::EnemyBullet)
		)
	{
		return;
	}
	//Type Checking for what the bullet collided with
	if (otherType->GetType() == ObjectType::Wall)
	{
		if (!m_HasBouncedThisFrame)
		{
			Bounce(otherObject);
			SDL_Log("Bullet bounced. Bounce count: %d", m_BounceCount);
			m_HasBouncedThisFrame = true;
		}
		return;
	}
	// If the bullet is a player bullet and it hits an enemy tank or recognizer, damage the enemy
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
		SDL_Log("Enemy bullet hit player");
		return;
	}
}
// When a player bullet hits an enemy, this function is called to apply damage to the enemy's health component.
void dae::BulletComponent::EnemyHit(GameObject* otherObject)
{
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
							otherObject,
							&GetOwner()
						});

					SDL_Log("Adding score: %d", score->GetScoreValue());

				}
			}


		}
	GetOwner().Destroy();
}
// When an enemy bullet hits the player, this function is called to trigger a PlayerHit event and destroy the bullet.
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
// Bounce calculates the new direction of the bullet after colliding with a wall,
// based on the side of the wall it hit, and updates the bullet's velocity and position accordingly.
// It also increments the bounce count and destroys the bullet if it has bounced too many times.
// The bounce logic uses the overlap between the bullet and the wall to determine the normal of the collision and reflects the bullet's direction across that normal.
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
