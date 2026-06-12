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

	int LoadLevel(dae::Scene& scene, const std::vector<std::string>& level);
	void CreateWall(dae::Scene& scene, glm::vec2 position);

	glm::vec2 GetPlayerStartPos(const std::vector<std::string>& level);
	glm::vec2 GetPlayerTwoStartPos(const std::vector<std::string>& level);
	glm::vec2 GetVersusPlayerStartPos(const std::vector<std::string>& level);

	void CreateEnemyTank(dae::Scene& scene, glm::vec2 position, const std::vector<std::string>& level);
	void CreateEnemyRecognizer(dae::Scene& scene, glm::vec2 position, const std::vector<std::string>& level);
	void CreateDiamond(dae::Scene& scene, glm::vec2 pos);
	PlayerObjects  CreatePlayer(dae::Scene& scene, glm::vec2 position, const std::vector<std::string>& level);
	PlayerObjects CreatePlayerTwo(dae::Scene& scene, glm::vec2 position, const std::vector<std::string>& level);
	PlayerObjects CreateVersusPlayer(dae::Scene& scene, glm::vec2 position);

}