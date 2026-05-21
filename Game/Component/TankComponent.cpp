#include "TankComponent.h"

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
