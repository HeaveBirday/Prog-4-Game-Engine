#include "ShooterComponent.h"
#include "ResourceManager.h"
#include <SceneManager.h>
#include <Scene.h>
#include <Components/RenderComponent.h>
#include <Components/VelocityComponent.h>
#include "BulletComponent.h"
#include <Texture2D.h>
#include <Components/CollisionComponent.h>
#include <ServiceLocator.h>
#include "../SoundIds.h"
#include "TankComponent.h"
#include "../GameCollisionLayer.h"
#include "ObjectTypeComponent.h"
void dae::ShooterComponent::Update(float)
{
}

void dae::ShooterComponent::Shoot()
{
	// if the object is destroyed OR isnt a tank it should return early and not shoot
	if (GetOwner().IsDestroyed())
	{
		SDL_Log("Tank is destroyed");
		return;
	}
	if (!m_TankComponent) return;
	if (!m_TankComponent->CanShoot()) return;
	m_TankComponent->ResetShootCooldown();
	auto transform = GetOwner().GetTransform();
	if (!transform) return; 

	//Setting up the direction and spawn position of the bullet based on the rotation of the tank
	glm::vec2 bulletDirection{};
	if (m_UseTurretDirection)
	{
		bulletDirection = m_TankComponent->GetTurretDirection();
	}
	else
	{
		bulletDirection = m_TankComponent->GetForwardDirection();
	}

	//Creating the bullet GameObject and setting the texture, based on the type of bullet
	auto bulletGameObject = std::make_unique<GameObject>();
	std::shared_ptr<Texture2D> texture;
	if (m_BulletType == ObjectType::PlayerBullet)
	{
		texture = ResourceManager::GetInstance().LoadTexture("BulletPlayer.png");
	}
	else if (m_BulletType == ObjectType::EnemyBullet)
	{
		texture = ResourceManager::GetInstance().LoadTexture("BulletEnemy.png");
	}
	else
	{
		texture = ResourceManager::GetInstance().LoadTexture("BulletPlayer.png");
	}

	bulletGameObject->AddComponent<CollisionComponent>(texture->GetSize(), dae::GameCollisionLayers::Bullet);

	//Calculating the spawn position of the bullet based on the center of the tank and the direction its facing, so it spawns in front of the tank instead of inside it
	glm::vec2 tankSize = m_TankComponent->GetSize();
	glm::vec2 bulletSize = texture->GetSize();
	glm::vec2 tankCenter{
		transform->GetX() + tankSize.x / 2.f,
		transform->GetY() + tankSize.y / 2.f
	};
	glm::vec2 bulletCenter{
		tankCenter + bulletDirection * ((tankSize.x / 2.f) + (bulletSize.x / 2.f)) + 4.f};
	glm::vec2 bulletSpawnPos{
		bulletCenter - bulletSize / 2.f
	};
	bulletGameObject->SetPosition(bulletSpawnPos);
	
	bulletGameObject->AddComponent<ObjectTypeComponent>(m_BulletType);
	bulletGameObject->AddComponent<RenderComponent>(texture);
	bulletGameObject->AddComponent<VelocityComponent>(200.f);
	bulletGameObject->AddComponent<BulletComponent>(200.f, bulletDirection);
	
	auto& scene = SceneManager::GetInstance().GetActiveScene();
	scene.Add(std::move(bulletGameObject));
	dae::ServiceLocator::GetSoundSystem().Play(dae::SoundIds::GunShot, 1.0f);

}
