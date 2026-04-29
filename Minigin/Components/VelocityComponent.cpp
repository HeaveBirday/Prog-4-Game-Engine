#include "VelocityComponent.h"
#include "Components/TransformComponent.h"
#include "GameObject.h"

void dae::VelocityComponent::Update(float dt)
{
	if (m_Direction == glm::vec2{ 0.f, 0.f })
		return;

	GameObject& owner = GetOwner();
	auto* transform = owner.GetTransform();
	
	glm::vec2 pos{ transform->GetX(), transform->GetY() };
	pos += m_Direction * m_Speed * dt;


	owner.SetPosition(pos.x, pos.y);

	m_Direction = glm::vec2{ 0.f, 0.f };
}


