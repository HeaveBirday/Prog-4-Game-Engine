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
#include "../Game/Component/EnemyAIComponent.h"
#include <Components/VelocityComponent.h>
#include "../Game/Component/PlayerTouchHitComponent.h"

static const std::vector<std::string> s_Level1 =
{
	"################################",
	"#__________#______________#____#",
	"#__________#____######____######",
	"#__#__###_______#____#___R_____#",
	"#__#__#_#_______#______________#",
	"#_____#_#_______________###__###",
	"#_____#_#####__________#__#____#",
	"#__#__#_#####______#___####____#",
	"#__#__#_#________D_#_________###",
	"#_____###______#####___________#",
	"#__________E___________________#",
	"#__###_________________##__#####",
	"#__#_____#####____#_____#______#",
	"#__#_____#_______##_____#______#",
	"#__##__###________#_____#####__#",
	"#________________##__E_________#",
	"#______________________________#",
	"################################",

};
static const std::vector<std::string> s_Level2 =
{
	"################################",
	"#__________#______________#____#",
	"#__________#____######____######",
	"#__#__###_______#____#_________#",
	"#__#__#_#_______#______________#",
	"#_____#_#_______________###__###",
	"#_____#_#####__________#__#____#",
	"#__#__#_#####____###___####____#",
	"#__#__#_#________#_#_________###",
	"#_____###______#####___R_______#",
	"#__________E___________________#",
	"#__###_________________##__#####",
	"#__#_____#####____#_____#______#",
	"#__#_____#_______##_____#______#",
	"#__##__###________#_____#####__#",
	"#________________##____________#",
	"#______________________________#",
	"################################",

};
static const std::vector<std::string> s_Level3 =
{
	"################################",
	"#__________#______________#____#",
	"#__________#____######____######",
	"#__#__###_______#____#_________#",
	"#__#__#_#_______#______________#",
	"#_____#_#_______________###__###",
	"#_____#_#####__________#__#____#",
	"#__#__#_#####____###___####____#",
	"#__#__#_#________#_#_________###",
	"#_____###______#####___________#",
	"#______________________________#",
	"#__###_________________##__#####",
	"#__#_____#####____#_____#______#",
	"#__#_____#_______##_____#______#",
	"#__##__###________#_____#####__#",
	"#______E_____R___##____________#",
	"#______________________________#",
	"################################",

};
const std::vector<std::string>& tron::GetLevel1()
{
	return s_Level1;
}

const std::vector<std::string>& tron::GetLevel2()
{
	return s_Level2;
}

const std::vector<std::string>& tron::GetLevel3()
{
	return s_Level3;
}

int tron::LoadLevel1(dae::Scene& scene)
{
	const float tileSize = 32.f;

	
	int enemyCount = 0;
	for (int yPos = 0; yPos < static_cast<int>(s_Level1.size()); ++yPos)
	{
		for (int xPos = 0; xPos < static_cast<int>(s_Level1[yPos].size()); ++xPos)
		{
			glm::vec2 pos{
				xPos * tileSize,
				yPos * tileSize
			};

			switch (s_Level1[yPos][xPos])
			{
			case '#':
				CreateWall(scene, pos);
				break;
			case 'E':
				CreateEnemyTank(scene, pos, s_Level1);
				enemyCount++;
				break;
			case 'R':
				CreateEnemyRecognizer(scene, pos, s_Level1);
				enemyCount++;
				break;
			case 'D':
				CreateDiamond(scene, pos);
				break;
			}
		}
	}
	return enemyCount;
}

int tron::LoadLevel2(dae::Scene& scene)
{
	const float tileSize = 32.f;

	
	int enemyCount = 0;
	for (int yPos = 0; yPos < static_cast<int>(s_Level2.size()); ++yPos)
	{
		for (int xPos = 0; xPos < static_cast<int>(s_Level2[yPos].size()); ++xPos)
		{
			glm::vec2 pos{
				xPos * tileSize,
				yPos * tileSize
			};

			switch (s_Level2[yPos][xPos])
			{
			case '#':
				CreateWall(scene, pos);
				break;
			case 'E':
				CreateEnemyTank(scene, pos, s_Level2);
				enemyCount++;
				break;
			case 'R':
				CreateEnemyRecognizer(scene, pos, s_Level2);
				enemyCount++;
				break;
			}
		}
	}
	return enemyCount;
}

int tron::LoadLevel3(dae::Scene& scene)
{
	const float tileSize = 32.f;

	
	int enemyCount = 0;
	for (int yPos = 0; yPos < static_cast<int>(s_Level3.size()); ++yPos)
	{
		for (int xPos = 0; xPos < static_cast<int>(s_Level3[yPos].size()); ++xPos)
		{
			glm::vec2 pos{
				xPos * tileSize,
				yPos * tileSize
			};

			switch (s_Level3[yPos][xPos])
			{
			case '#':
				CreateWall(scene, pos);
				break;
			case 'E':
				CreateEnemyTank(scene, pos, s_Level3);
				enemyCount++;
				break;
			case 'R':
				CreateEnemyRecognizer(scene, pos, s_Level3);
				enemyCount++;
				break;
			}
		}
	}
	return enemyCount;
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

void tron::CreateEnemyTank(dae::Scene& scene, glm::vec2 position, const std::vector<std::string>& level)
{
	auto blueTankTexture = dae::ResourceManager::GetInstance().LoadTexture("BlueTank.png");
	auto blueTank = std::make_unique<dae::GameObject>();
	blueTank->SetPosition(position);

	blueTank->AddComponent<ObjectTypeComponent>(ObjectType::EnemyTank);
	blueTank->AddComponent<ScoreComponent>(100);
	blueTank->AddComponent<HealthComponent>(3);
	blueTank->AddComponent<dae::RenderComponent>(blueTankTexture);
	blueTank->AddComponent<dae::CollisionComponent>(glm::vec2{ 32.f,32.f }, dae::GameCollisionLayers::Tank);
	blueTank->AddComponent<dae::VelocityComponent>(120.f);
	blueTank->AddComponent<dae::TankComponent>(blueTankTexture->GetSize());
	blueTank->AddComponent<dae::ShooterComponent>(ObjectType::EnemyBullet);

	blueTank->AddComponent<dae::EnemyAIComponent>(level);

	scene.Add(std::move(blueTank));


}

void tron::CreateEnemyRecognizer(dae::Scene& scene, glm::vec2 position, const std::vector<std::string>& level)
{
	auto recognizerTexture = dae::ResourceManager::GetInstance().LoadTexture("Recognizer.png");
	auto recognizer = std::make_unique<dae::GameObject>();
	recognizer->SetPosition(position);

	recognizer->AddComponent<ObjectTypeComponent>(ObjectType::Recognizer);
	recognizer->AddComponent<ScoreComponent>(250);
	recognizer->AddComponent<HealthComponent>(3);
	recognizer->AddComponent<dae::RenderComponent>(recognizerTexture);
	recognizer->AddComponent<dae::CollisionComponent>(glm::vec2{ 32.f,32.f }, dae::GameCollisionLayers::Tank);
	recognizer->AddComponent<dae::VelocityComponent>(240.f);
	recognizer->AddComponent<dae::TankComponent>(recognizerTexture->GetSize());

	recognizer->AddComponent<dae::EnemyAIComponent>(level);

	scene.Add(std::move(recognizer));
}

void tron::CreateDiamond(dae::Scene& scene, glm::vec2 pos)
{
	auto diamondTexture = dae::ResourceManager::GetInstance().LoadTexture("Diamond.png");
	auto diamond = std::make_unique<dae::GameObject>();
	diamond->SetPosition(pos);

	diamond->AddComponent<ObjectTypeComponent>(ObjectType::Diamond);
	auto& render = diamond->AddComponent<dae::RenderComponent>(diamondTexture);
	render.SetScale(32.f / diamondTexture->GetSize());

	diamond->AddComponent<dae::CollisionComponent>(glm::vec2{ 32.f, 32.f },
		dae::GameCollisionLayers::Tank);
	scene.Add(std::move(diamond));

}

PlayerObjects  tron::CreatePlayer(dae::Scene& scene, glm::vec2 position, const std::vector<std::string>& level)
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

	player->AddComponent<dae::VelocityComponent>(120.f);
	player->AddComponent<dae::CollisionComponent>(
		glm::vec2{ 32.f, 32.f },
		dae::GameCollisionLayers::Tank);
	player->AddComponent<dae::PlayerTouchHitComponent>(level);
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
