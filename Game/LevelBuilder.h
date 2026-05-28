#pragma once
#include <glm/glm.hpp>
#include <GameObject.h>

namespace dae
{
	class Scene;
}
struct PlayerObjects
{
	dae::GameObject* player{};
	dae::GameObject* turret{};
};
namespace tron
{
	int LoadLevel1(dae::Scene& scene);
	void CreateWall(dae::Scene& scene, glm::vec2 position);

	void CreateEnemyTank(dae::Scene& scene, glm::vec2 position);
	PlayerObjects  CreatePlayer(dae::Scene& scene, glm::vec2 position);
}