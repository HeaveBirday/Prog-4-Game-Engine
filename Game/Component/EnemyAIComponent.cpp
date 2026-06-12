#include "EnemyAIComponent.h"
#include <SDL3/SDL.h>
#include <random>
#include "GameObject.h"
#include "ShooterComponent.h"
#include <glm/geometric.hpp>
#include "TankComponent.h"

dae::EnemyAIComponent::EnemyAIComponent(GameObject* owner, const std::vector<std::string>& level, float speed)
	:
	Component(owner), 
	m_Level(level),
	m_Speed(speed),
	m_Shooter(owner->GetComponent<ShooterComponent>()),
	m_Transform(owner->GetComponent<TransformComponent>()),
	m_Tank(owner->GetComponent<TankComponent>())
{
	PickRandomDirection();

	m_TargetPosition =
	{
		m_Transform->GetWorldPosition()
	};

	m_ShootTimer = m_ShootInterval;
}

void dae::EnemyAIComponent::Update(float dt)
{
	//Movement
	glm::vec2 currentPos = m_Transform->GetWorldPosition();

	if (!m_IsMovingToTile)
	{
		glm::ivec2 currentTile = GetCurrentTile();
		glm::ivec2 nextTile{
			currentTile.x + static_cast<int>(m_CurrentDirection.x),
			currentTile.y + static_cast<int>(m_CurrentDirection.y)
		};

		if (IsWallAtTile(nextTile.x, nextTile.y))
		{
			PickRandomDirection();
			return;
		}
		m_TargetPosition = {
			nextTile.x * m_TileSize, 
			nextTile.y * m_TileSize
		};

		m_IsMovingToTile = true;
	}

	glm::vec2 toTarget = m_TargetPosition - currentPos; 
	float distance = glm::length(toTarget);
	float step = m_Speed * dt;

	if (distance <= step)
	{
		m_Transform->SetPosition(m_TargetPosition.x, m_TargetPosition.y);
		m_IsMovingToTile = false;

		if (rand() % 4 == 0)
		{
			PickRandomDirection();
		}
	}
	else
	{
		glm::vec2 moveDirection = glm::normalize(toTarget);
		glm::vec2 newPos = currentPos + moveDirection * step;
		m_Transform->SetPosition(newPos);
	}

	//Shoot timer
	m_ShootTimer -= dt;
	if (m_ShootTimer <= 0.f)
	{
		if (m_Shooter)
		{
			m_Shooter->Shoot();
		}
		m_ShootTimer = m_ShootInterval;
	}
}

void dae::EnemyAIComponent::PickRandomDirection()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<> dist(0, 3);

	switch (dist(gen))
	{
	case 0:
		m_CurrentDirection = { 0.f, -1.f };
		break;
	case 1:
		m_CurrentDirection = { 0.f, 1.f };
		break;
	case 2:
		m_CurrentDirection = { -1.f, 0.f };
		break;
	case 3:
		m_CurrentDirection = { 1.f, 0.f };
		break;
	}
	if (m_Tank)
		m_Tank->SetMoveDirection(m_CurrentDirection);
}

bool dae::EnemyAIComponent::IsWallAtTile(int x, int y) const
{
	if (y < 0 || y >= static_cast<int>(m_Level.size())) return true;
	if (x < 0 || x >= static_cast<int>(m_Level[y].size())) return true;

	return m_Level[y][x] == '#';
}

glm::ivec2 dae::EnemyAIComponent::GetCurrentTile() const
{
	return{
		static_cast<int>(m_Transform->GetX() / m_TileSize),
		static_cast<int>(m_Transform->GetY() / m_TileSize)
	};
}
