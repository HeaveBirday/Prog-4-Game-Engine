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
	const std::vector<std::string>& GetLevel1();
	const std::vector<std::string>& GetLevel2();
	const std::vector<std::string>& GetLevel3();

	int LoadLevel1(dae::Scene& scene);
	int LoadLevel2(dae::Scene& scene);
	int LoadLevel3(dae::Scene& scene);
	void CreateWall(dae::Scene& scene, glm::vec2 position);

	void CreateEnemyTank(dae::Scene& scene, glm::vec2 position, const std::vector<std::string>& level);
	void CreateEnemyRecognizer(dae::Scene& scene, glm::vec2 position, const std::vector<std::string>& level);
	void CreateDiamond(dae::Scene& scene, glm::vec2 pos);
	PlayerObjects  CreatePlayer(dae::Scene& scene, glm::vec2 position, const std::vector<std::string>& level);
}