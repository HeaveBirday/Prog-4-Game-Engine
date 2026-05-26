#include "LevelBuilder.h"
#include <SDL3/SDL.h>
#include <vector>
#include <string>

#include <Scene.h>
#include <GameObject.h>
#include <ResourceManager.h>
#include <Components/RenderComponent.h>
#include <Components/CollisionComponent.h>
#include "../Game/GameCollisionLayer.h"
#include "../Game/Component/ObjectTypeComponent.h"
#include "../Game/Component/HealthComponent.h"
#include "../Game/Component/TankComponent.h"
#include "../Game/Component/ScoreComponent.h"

#include "../Game/Component/ShooterComponent.h"
#include "../Game/Component/LivesComponent.h"
#include "../Game/Component/PlayerScoreComponent.h"
#include "../Game/Component/WallCollisionComponent.h"
#include <Components/VelocityComponent.h>


void tron::LoadLevel1(dae::Scene& scene)
{
	const float tileSize = 32.f;

	const std::vector<std::string> level =
	{
		"################################",
		"#______________________________#",
		"#______________________________#",
		"#__#____#______________________#",
		"#__#____#______________________#",
		"#_______#______________________#",
		"#_______#####__________________#",
		"#__#____#___#__________________#",
		"#__#___________________________#",
		"#______________________________#",
		"#______________________________#",
		"#__###_________________________#",
		"#__#_____#####_________________#",
		"#__#_____#_____________________#",
		"#__##__###_____________________#",
		"#______________________________#",
		"#______________________________#",
		"################################",
		
	};

	for (int yPos = 0; yPos < static_cast<int>(level.size()); ++yPos)
	{
		for (int xPos = 0; xPos < static_cast<int>(level[yPos].size()); ++xPos)
		{
			glm::vec2 pos{
				xPos * tileSize,
				yPos * tileSize
			};

			switch (level[yPos][xPos])
			{
			case '#':
				CreateWall(scene, pos);
				break;
			case 'E':
				CreateEnemyTank(scene, pos);
				break;
			}
		}
	}
}

void tron::CreateWall(dae::Scene& scene, glm::vec2 position)
{
	auto wallTexture = dae::ResourceManager::GetInstance().LoadTexture("Wall.png");

	auto wall = std::make_unique<dae::GameObject>();
	wall->SetPosition(position);
	wall->AddComponent<ObjectTypeComponent>(ObjectType::Wall);
	
	auto& wallRender = wall->AddComponent<dae::RenderComponent>(wallTexture);
	wallRender.SetScale(32.f / wallTexture->GetSize());

	wall->AddComponent<dae::CollisionComponent>(glm::vec2{ 32.f, 32.f }, dae::GameCollisionLayers::Wall);
	scene.Add(std::move(wall));
}

void tron::CreateEnemyTank(dae::Scene& scene, glm::vec2 position)
{
	auto blueTankTexture = dae::ResourceManager::GetInstance().LoadTexture("BlueTank.png");
	auto blueTank = std::make_unique<dae::GameObject>();
	blueTank->SetPosition(position);

	blueTank->AddComponent<ObjectTypeComponent>(ObjectType::EnemyTank);
	blueTank->AddComponent<ScoreComponent>(100);
	blueTank->AddComponent<HealthComponent>(3);
	blueTank->AddComponent<dae::RenderComponent>(blueTankTexture);
	blueTank->AddComponent<dae::CollisionComponent>(glm::vec2{ 32.f,32.f }, dae::GameCollisionLayers::Tank);
	blueTank->AddComponent<dae::TankComponent>(blueTankTexture->GetSize());
	
	scene.Add(std::move(blueTank));


}

PlayerObjects  tron::CreatePlayer(dae::Scene& scene, glm::vec2 position)
{
	auto tankTexture = dae::ResourceManager::GetInstance().LoadTexture("RedTank.png");
	auto turretTexture = dae::ResourceManager::GetInstance().LoadTexture("RedTankGun.png");
	const glm::vec2 tankSize = tankTexture->GetSize();
	const glm::vec2 turretSize = turretTexture->GetSize();
	
	//creating turret first
	auto turret = std::make_unique<dae::GameObject>();
	turret->AddComponent<dae::RenderComponent>(turretTexture);
	auto* turretPtr = turret.get();
	scene.Add(std::move(turret));

	//Creating PlayerTank
	auto player = std::make_unique<dae::GameObject>();
	player->SetPosition(position);

	player->AddComponent<dae::WallCollisionComponent>();
	player->AddComponent<dae::RenderComponent>(tankTexture);
	player->AddComponent<ObjectTypeComponent>(ObjectType::Player);
	player->AddComponent<LivesComponent>(3);
	player->AddComponent<PlayerScoreComponent>();

	player->AddComponent<dae::VelocityComponent>(120.f);
	player->AddComponent<dae::CollisionComponent>(
		glm::vec2{ 32.f, 32.f },
		dae::GameCollisionLayers::Tank);

	player->AddComponent<dae::TankComponent>(tankSize);
	player->AddComponent<dae::ShooterComponent>(ObjectType::PlayerBullet);
	auto* playerPtr = player.get();
	scene.Add(std::move(player));

	//Attach turret to Player Tank
	turretPtr->SetParent(playerPtr, false);
	turretPtr->GetTransform()->SetLocalPosition(
		(tankSize.x - turretSize.x) / 2.f,
		(tankSize.y - turretSize.y) / 2.f);

	return { playerPtr, turretPtr };


}
