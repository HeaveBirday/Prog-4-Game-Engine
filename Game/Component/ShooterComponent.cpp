#include "ShooterComponent.h"
#include "ResourceManager.h"
#include <SceneManager.h>
#include <Scene.h>
#include <Components/RenderComponent.h>
#include <Components/VelocityComponent.h>
#include "BulletComponent.h"
#include <Texture2D.h>
#include <Components/CollisionComponent.h>

void dae::ShooterComponent::Update(float)
{
}

void dae::ShooterComponent::Shoot()
{
	auto bulletGameObject = std::make_unique<GameObject>();
	auto transform = GetOwner().GetTransform();
	
	bulletGameObject->SetPosition(transform->GetX(), transform->GetY());
	

	auto texture = ResourceManager::GetInstance().LoadTexture("BulletPlayer.png");
	bulletGameObject->AddComponent<CollisionComponent>(texture->GetSize());

	bulletGameObject->AddComponent<RenderComponent>(texture);
	bulletGameObject->AddComponent<VelocityComponent>(200.f);
	bulletGameObject->AddComponent<BulletComponent>(200.f, glm::vec2{ 0, -1 });
	
	auto& scene = SceneManager::GetInstance().GetActiveScene();
	scene.Add(std::move(bulletGameObject));

}
