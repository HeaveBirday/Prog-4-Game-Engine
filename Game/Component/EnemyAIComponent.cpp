#include "EnemyAIComponent.h"
#include <SDL3/SDL.h>
#include <random>
#include "GameObject.h"
#include <Components/VelocityComponent.h>
#include "TankComponent.h"
#include "ShooterComponent.h"

dae::EnemyAIComponent::EnemyAIComponent(GameObject* owner)
	: Component(owner), 
	m_Velocity(owner->GetComponent<VelocityComponent>()),
	m_Tank(owner->GetComponent<TankComponent>()),
	m_Shooter(owner->GetComponent<ShooterComponent>())
{
	m_DirectionTimer = m_DirectionChangeInterval;

	m_ShootTimer = m_ShootInterval;
	PickRandomDirection();
}

void dae::EnemyAIComponent::Update(float dt)
{
	if (!m_Velocity || !m_Tank) return;

	if (m_WallCooldown > 0.f)
		m_WallCooldown -= dt;

	m_DirectionTimer -= dt;

	if (m_DirectionTimer <= 0.f)
	{
		PickRandomDirection();
		m_DirectionTimer = m_DirectionChangeInterval;
	}

	MoveCommand moveCommand(*m_Velocity, *m_Tank, m_CurrentDirection);
	moveCommand.Execute(dt);

	m_ShootTimer -= dt;

	if (m_ShootTimer <= 0.f)
	{
		if (m_Shooter)
		{
			m_Shooter->Shoot();
			SDL_Log("Enemy tries to shoot");
		}

		m_ShootTimer = m_ShootInterval;
	}
}

void dae::EnemyAIComponent::PickRandomDirection()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<> dist(0, 3);

	glm::vec2 newDir{};

	do
	{
		const int randomDirection = dist(gen);

		switch (randomDirection)
		{
		case 0:
			newDir = { 0.f, -1.f };
			break;
		case 1:
			newDir = { 0.f, 1.f };
			break;
		case 2:
			newDir = { -1.f, 0.f };
			break;
		case 3:
			newDir = { 1.f, 0.f };
			break;
		}

	}
	while (newDir == m_CurrentDirection);

	m_CurrentDirection = newDir;
}

void dae::EnemyAIComponent::OnWallHit()
{
	if (m_WallCooldown > 0.f) return;

	PickRandomDirection();
	m_DirectionTimer = m_DirectionChangeInterval;
	m_WallCooldown = 0.25f;

}