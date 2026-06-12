#pragma once
#include <Components/Component.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
namespace dae
{

	class ShooterComponent;
	class TransformComponent;
	class TankComponent;
	class EnemyAIComponent final : public Component
	{
	public:
		explicit EnemyAIComponent(GameObject* owner, const std::vector<std::string>& level, float speed);
		void Update(float dt) override;

	private:
		void PickRandomDirection();
		bool IsWallAtTile(int x, int y) const;
		glm::ivec2 GetCurrentTile() const;
		const std::vector<std::string>& m_Level;
		float m_Speed;

		ShooterComponent* m_Shooter{};
		TransformComponent* m_Transform{};
		TankComponent* m_Tank{};
		glm::vec2 m_CurrentDirection{ 1.f, 0.f };
		glm::vec2 m_TargetPosition{};
		bool m_IsMovingToTile{false};
		
		float m_TileSize{ 32.f };

		float m_ShootTimer{};
		float m_ShootInterval{2.f};
	};
}