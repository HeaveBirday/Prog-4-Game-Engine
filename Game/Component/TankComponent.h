#pragma once
#include "Components/Component.h"
#include <glm/glm.hpp>


namespace dae
{
	// The Direction enum represents the four cardinal directions that a tank can face or move towards. 
	// It is used by the TankComponent to determine the tank's orientation and movement direction.
	enum class Direction
	{
		Up,
		Down,
		Left,
		Right
	};
	// The TankComponent is responsible for managing the state and behavior of a tank GameObject, including its size, direction, turret rotation, and shooting cooldown.
	class TankComponent final : public Component
	{
	public:
		explicit TankComponent(GameObject* owner, glm::vec2 size)
			: Component(owner),
			m_Size(size)
		{

		}
		void SetDirection(Direction direction)
		{
			m_Direction = direction;
		}
		void SetMoveDirection(glm::vec2 direction);
		Direction GetDirection() const
		{
			return m_Direction;
		}
		glm::vec2 GetSize() const
		{
			return m_Size;
		}
		glm::vec2 GetForwardDirection() const;

		void RotateTurret(float angleData);
		float GetTurretAngle() const;
		glm::vec2 GetTurretDirection() const;
		void Update(float dt) override;
		bool CanShoot() const;
		void ResetShootCooldown();
	private:
		Direction m_Direction{ Direction::Up };
		glm::vec2 m_Size{};
		float m_ShootCooldown{};
		float m_TurretAngle{};
	};
	
}