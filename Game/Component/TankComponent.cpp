#include "TankComponent.h"
#include <GameObject.h>
#include <Components/TransformComponent.h>

glm::vec2 dae::TankComponent::GetForwardDirection() const
{
	switch (m_Direction)
	{
	case Direction::Up:
		return { 0.f, -1.f };
	case Direction::Down:
		return { 0.f, 1.f };
	case Direction::Left:
		return { -1.f, 0.f };
	case Direction::Right:
		return { 1.f, 0.f };
	}

	return { 0.f, -1.f };
	
}

void dae::TankComponent::RotateTurret(float angleData)
{
	m_TurretAngle += angleData;

	if (m_TurretAngle >= 360.f) m_TurretAngle -= 360.f;
	if (m_TurretAngle < 0.f) m_TurretAngle += 360.f;
}

float dae::TankComponent::GetTurretAngle() const
{
	return m_TurretAngle;
}
// This function calculates the direction vector of the turret based on its current angle. 
// It converts the angle from degrees to radians and 
// then uses cosine and sine functions to determine the x and y components of the direction vector, respectively.
glm::vec2 dae::TankComponent::GetTurretDirection() const
{
	const float radians = m_TurretAngle * 3.141f / 180.f;
	return
	{
		std::cos(radians),
		std::sin(radians)
	};
}

void dae::TankComponent::Update(float dt)
{
	if (m_ShootCooldown > 0.f)
	{
		m_ShootCooldown -= dt;
	}
}

bool dae::TankComponent::CanShoot() const
{
	return m_ShootCooldown <= 0.f;
}

void dae::TankComponent::ResetShootCooldown()
{
	m_ShootCooldown = 0.5f; 
}
// This function sets the movement direction of the tank based on the input vector and updates the tank's rotation accordingly.
void dae::TankComponent::SetMoveDirection(glm::vec2 direction)
{
	if (direction == glm::vec2{ 0.f, 0.f })
		return;

	if (direction == glm::vec2{ 0.f, -1.f })
	{
		SetDirection(Direction::Up);
		GetOwner().GetTransform()->SetRotation(270.f);
	}
	else if (direction == glm::vec2{ 0.f, 1.f })
	{
		SetDirection(Direction::Down);
		GetOwner().GetTransform()->SetRotation(90.f);
	}
	else if (direction == glm::vec2{ -1.f, 0.f })
	{
		SetDirection(Direction::Left);
		GetOwner().GetTransform()->SetRotation(180.f);
	}
	else if (direction == glm::vec2{ 1.f, 0.f })
	{
		SetDirection(Direction::Right);
		GetOwner().GetTransform()->SetRotation(0.f);
	}
}
