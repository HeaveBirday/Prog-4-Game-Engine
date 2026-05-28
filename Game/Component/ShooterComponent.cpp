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
	auto* objectType = GetOwner().GetComponent<ObjectTypeComponent>();
	if (objectType && objectType->GetType() == ObjectType::Player)
	{
		bulletDirection = m_TankComponent->GetTurretDirection();
	}
	else
	{
		bulletDirection = m_TankComponent->GetForwardDirection();
	}
	auto bulletGameObject = std::make_unique<GameObject>();

	auto texture = ResourceManager::GetInstance().LoadTexture("BulletPlayer.png");
	bulletGameObject->AddComponent<CollisionComponent>(texture->GetSize(), dae::GameCollisionLayers::Bullet);

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
