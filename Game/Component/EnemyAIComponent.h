#pragma once
#include <Components/Component.h>
#include <glm/glm.hpp>
#include <../Game/Commands/MoveCommand.h>

namespace dae
{
	class VelocityComponent;
	class TankComponent;
	class ShooterComponent;

	class EnemyAIComponent final : public Component
	{
	public:
		explicit EnemyAIComponent(GameObject* owner);
		void Update(float dt) override;
		void OnWallHit();

	private:
		void PickRandomDirection();

		VelocityComponent* m_Velocity{};
		TankComponent* m_Tank{};
		ShooterComponent* m_Shooter{};

		glm::vec2 m_CurrentDirection{};

		float m_DirectionTimer{};
		float m_DirectionChangeInterval{ 1.f };
		float m_WallCooldown{};


		float m_ShootTimer{};
		float m_ShootInterval{ 2.f };
	};
}