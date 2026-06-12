#pragma once
#include <Components/Component.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

// EnemyAIComponent controls the behavior of enemy tanks, including movement and shooting. It uses the level layout to navigate and avoid walls.
namespace dae
{

	class ShooterComponent;
	class TransformComponent;
	class TankComponent;
	// EnemyAIComponent makes the enemy tank move in a random direction until it hits a wall, at which point it picks a new random direction. It also periodically shoots at the player.
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