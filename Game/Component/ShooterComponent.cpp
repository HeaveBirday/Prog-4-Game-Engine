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
void dae::ShooterComponent::Update(float)
{
}

void dae::ShooterComponent::Shoot()
{
	if (!m_TankComponent) return;

	auto transform = GetOwner().GetTransform();
	if (!transform) return; 
	//Setting up the direction and spawn position of the bullet based on the rotation of the tank
	glm::vec2 bulletDirection{};
	const float rotation = transform->GetRotation();
	if (rotation == 0.f)
	{
		bulletDirection = { 1.f, 0.f };
	}
	else if(rotation == 90.f)
	{
		bulletDirection = { 0.f, 1.f };
	}
	else if(rotation == 180.f)
	{
		bulletDirection = { -1.f, 0.f };
	}
	else if(rotation == 270.f)
	{
		bulletDirection = { 0.f, -1.f };
	}
	auto bulletGameObject = std::make_unique<GameObject>();
	auto texture = ResourceManager::GetInstance().LoadTexture("BulletPlayer.png");

	glm::vec2 tankSize = m_TankComponent->GetSize();
	glm::vec2 bulletSize = texture->GetSize();
	glm::vec2 tankCenter{
		transform->GetX() + tankSize.x / 2.f,
		transform->GetY() + tankSize.y / 2.f
	};
	glm::vec2 bulletCenter{
		tankCenter + bulletDirection * (tankSize.x / 2.f)};
	glm::vec2 bulletSpawnPos{
		bulletCenter - bulletSize / 2.f
	};
	bulletGameObject->SetPosition(bulletSpawnPos);
	bulletGameObject->AddComponent<CollisionComponent>(texture->GetSize());

	bulletGameObject->AddComponent<RenderComponent>(texture);
	bulletGameObject->AddComponent<VelocityComponent>(200.f);
	bulletGameObject->AddComponent<BulletComponent>(200.f, bulletDirection);
	
	auto& scene = SceneManager::GetInstance().GetActiveScene();
	scene.Add(std::move(bulletGameObject));
	dae::ServiceLocator::GetSoundSystem().Play(dae::SoundIds::GunShot, 1.0f);

}
