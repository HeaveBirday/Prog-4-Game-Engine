#include "VelocityComponent.h"
#include "Components/TransformComponent.h"
#include "GameObject.h"
#include "../../Game/Component/BulletComponent.h"
#include <stdexcept>

void dae::VelocityComponent::Update(float dt)
{
	if (m_Direction == glm::vec2{ 0.f, 0.f })
		return;

	GameObject& owner = GetOwner();
	auto* transform = owner.GetTransform();

	glm::vec2 pos{ transform->GetX(), transform->GetY() };
	pos += m_Direction * m_Speed * dt;

	if (pos.x > 1024 || pos.x < 0 || pos.y > 576 || pos.y < 0)
	{
		int test = 5;
		test += 30;
	}

	owner.SetPosition(pos.x, pos.y);
	
}


