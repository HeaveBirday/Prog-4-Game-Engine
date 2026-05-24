#include "TankComponent.h"

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
