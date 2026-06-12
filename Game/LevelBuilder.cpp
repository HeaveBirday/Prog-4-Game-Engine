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
#include "../Game/Component/WallCollisionComponent.h"
#include "../Game/Component/EnemyAIComponent.h"
#include <Components/VelocityComponent.h>
#include "../Game/Component/PlayerTouchHitComponent.h"

static const std::vector<std::string> s_Level2 =
{
	"################################",
	"################################",
	"#____________#_________________#",
	"#_#___###__######____#___R_____#",
	"#_#___#_#_______#____#_________#",
	"#_#___#_#_______###_____###__###",
	"#__V__#_#####__________#__#____#",
	"###___#_#####______#__#####____#",
	"#_#___#_#________D_#____#____###",
	"#_____###______#####__######___#",
	"#__________E________________P__#",
	"#_#_##______________#####_######",
	"#_#______#####____#_____#______#",
	"#_#______________##_____#______#",
	"#_#_#__###________#_____#####__#",
	"#_#_________2____##__E_________#",
	"#______________________________#",
	"################################",

};
static const std::vector<std::string> s_Level3 =
{
	"################################",
	"########################D____P_##",
	"#__2_________________###########",
	"#_#___###_______#____#___R_____#",
	"#_#___#_##______####___________#",
	"#_#___#_________#____######____#",
	"#_#___#____##___#______#__#__#_#",
	"#_#___#___###___####___#_____#_#",
	"#_R___#_##_#_____D#____#__##_#_#",
	"###___###__#_______#___#__#__#_#",
	"#_V________E__###__#___________#",
	"#_#_##_________________##__#####",
	"#_#______#####____#_____#______#",
	"#_#______#_#_#_____####_#______#",
	"#_#_#__###____________#____##__#",
	"#_#___#____#_____##__E_________#",
	"#__E__#____#___________________#",
	"################################",

};
static const std::vector<std::string> s_Level1 =
{
	"################################",
	"################################",
	"################################",
	"#__#__###_______#____#_________#",
	"#__#__#_#_______#______________#",
	"#_____#_#______R________###__###",
	"#__P__#_#####__________#__#____#",
	"#__#__#_#####______#___####____#",
	"#__#__#_#________D_#_________###",
	"#_____###______#####___________#",
	"#__________________V_______2___#",
	"#__###_________________##__#####",
	"#__#_____#####____#_____#______#",
	"#__#_____#_______#####__#______#",
	"#__##__###________#__#__#####__#",
	"#________#____E__##__#_________#",
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
// The LoadLevel function reads the level layout from a vector of strings, where each character represents a different type of object (wall, enemy, player start position, etc.).
// It creates the corresponding GameObjects and adds them to the scene. It also counts the number of enemies in the level and returns that count.
int tron::LoadLevel(dae::Scene& scene, const std::vector<std::string>& level)
{
	const float tileSize = 32.f;
	int enemyCount = 0;

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
				CreateEnemyTank(scene, pos, level);
				enemyCount++;
				break;
			case 'R':
				CreateEnemyRecognizer(scene, pos, level);
				enemyCount++;
				break;
			case 'D':
				CreateDiamond(scene, pos);
				break;
			case 'P':
				break;
			case '2':
				break;
			case 'V':
				break;
			}
		}
	}

	return enemyCount;
}

// The CreateWall function creates a wall GameObject at the specified position, with the appropriate texture, collision component, and object type component. It then adds the wall to the scene.
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
// The GetPlayerStartPos function scans the level layout for the character 'P', which represents the player's starting position. 
// It returns the corresponding world coordinates based on the tile size. If no 'P' is found, it returns a default position.
glm::vec2 tron::GetPlayerStartPos(const std::vector<std::string>& level)
{
	const float tileSize = 32.f;

	for (int y = 0; y < static_cast<int>(level.size()); ++y)
	{
		for (int x = 0; x < static_cast<int>(level[y].size()); ++x)
		{
			if (level[y][x] == 'P')
			{
				return glm::vec2{
					x * tileSize,
					y * tileSize
				};
			}
		}
	}

	return glm::vec2{ 448.f, 192.f };
}
// The GetPlayerTwoStartPos function scans the level layout for the character '2', which represents the second player's starting position.
glm::vec2 tron::GetPlayerTwoStartPos(const std::vector<std::string>& level)
{
	
	const float tileSize = 32.f;

	for (int y = 0; y < static_cast<int>(level.size()); ++y)
	{
		for (int x = 0; x < static_cast<int>(level[y].size()); ++x)
		{
			if (level[y][x] == '2')
			{
				return glm::vec2{
					x * tileSize,
					y * tileSize
				};
			}
		}
	}

	return glm::vec2{ 512.f, 192.f };
}
// The GetVersusPlayerStartPos function scans the level layout for the character 'V', which represents the versus player's starting position.
glm::vec2 tron::GetVersusPlayerStartPos(const std::vector<std::string>& level)
{
	
	const float tileSize = 32.f;

	for (int y = 0; y < static_cast<int>(level.size()); ++y)
	{
		for (int x = 0; x < static_cast<int>(level[y].size()); ++x)
		{
			if (level[y][x] == 'V')
			{
				return glm::vec2{
					x * tileSize,
					y * tileSize
				};
			}
		}
	}

	return glm::vec2{ 512.f, 192.f };
}
// The CreateEnemyTank function creates an enemy tank GameObject at the specified position, with the appropriate texture,
// components for rendering, collision, health, score value, and AI behavior. It then adds the enemy tank to the scene.
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
	blueTank->AddComponent<dae::TankComponent>(blueTankTexture->GetSize());
	blueTank->AddComponent<dae::ShooterComponent>(ObjectType::EnemyBullet);

	blueTank->AddComponent<dae::EnemyAIComponent>(level, 120.f);

	scene.Add(std::move(blueTank));
}
// The CreateEnemyRecognizer function creates an enemy recognizer GameObject at the specified position, with the appropriate texture.
// Basically an EnemyTank with higher speed and score value, but no shooting component
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
	recognizer->AddComponent<dae::TankComponent>(recognizerTexture->GetSize());

	recognizer->AddComponent<dae::EnemyAIComponent>(level, 240.f);

	scene.Add(std::move(recognizer));
}
// The CreateDiamond function creates a diamond GameObject at the specified position, with the appropriate texture, collision component, and object type component. It then adds the diamond to the scene.
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
// The CreatePlayer function creates the player tank GameObject at the specified position, with the appropriate texture, 
// components for rendering, collision, health, score value, and shooting behavior. 
// It also creates a turret GameObject as a child of the player tank, which visually represents the player's gun. 
// Finally, it adds both the player tank and turret to the scene and returns pointers to them.
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
		glm::vec2{ 29.f, 29.f },
		dae::GameCollisionLayers::Tank);
	player->AddComponent<dae::PlayerTouchHitComponent>(level);
	player->AddComponent<dae::TankComponent>(tankSize);
	player->AddComponent<dae::ShooterComponent>(ObjectType::PlayerBullet, true);
	auto* playerPtr = player.get();
	scene.Add(std::move(player));

	//Attach turret to Player Tank
	turretPtr->SetParent(playerPtr, false);
	turretPtr->GetTransform()->SetLocalPosition(
		(tankSize.x - turretSize.x) / 2.f,
		(tankSize.y - turretSize.y) / 2.f);

	return { playerPtr, turretPtr };


}
// The CreatePlayerTwo function is similar to CreatePlayer, but it creates a second player tank with a different texture (green instead of red).
PlayerObjects tron::CreatePlayerTwo(dae::Scene& scene, glm::vec2 position, const std::vector<std::string>& level)
{
	
	auto tankTexture = dae::ResourceManager::GetInstance().LoadTexture("GreenTank.png");
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
		glm::vec2{ 29.f, 29.f },
		dae::GameCollisionLayers::Tank);
	player->AddComponent<dae::PlayerTouchHitComponent>(level);
	player->AddComponent<dae::TankComponent>(tankSize);
	player->AddComponent<dae::ShooterComponent>(ObjectType::PlayerBullet, true);
	auto* playerPtr = player.get();
	scene.Add(std::move(player));

	//Attach turret to Player Tank
	turretPtr->SetParent(playerPtr, false);
	turretPtr->GetTransform()->SetLocalPosition(
		(tankSize.x - turretSize.x) / 2.f,
		(tankSize.y - turretSize.y) / 2.f);

	return { playerPtr, turretPtr };
}
// The CreateVersusPlayer function is also similar to CreatePlayer and CreateEnemyTank. It pretty much creates an enemy tank, with a different texture and a turret GameObject, 
// but it's meant to be controlled by a second player instead of AI. It also returns pointers to the player tank and turret for input binding.

PlayerObjects tron::CreateVersusPlayer(dae::Scene& scene, glm::vec2 position)
{
	auto pinkTankTexture = dae::ResourceManager::GetInstance().LoadTexture("PinkTank.png");
	auto turretTexture = dae::ResourceManager::GetInstance().LoadTexture("RedTankGun.png");
	auto pinkTank = std::make_unique<dae::GameObject>();
	const glm::vec2 tankSize = pinkTankTexture->GetSize();
	const glm::vec2 turretSize = turretTexture->GetSize();
	//creating turret first
	auto turret = std::make_unique<dae::GameObject>();
	turret->AddComponent<dae::RenderComponent>(turretTexture);
	auto* turretPtr = turret.get();
	scene.Add(std::move(turret));

	pinkTank->SetPosition(position);

	pinkTank->AddComponent<ObjectTypeComponent>(ObjectType::EnemyTank);
	pinkTank->AddComponent<ScoreComponent>(100);
	pinkTank->AddComponent<HealthComponent>(3);
	pinkTank->AddComponent<dae::WallCollisionComponent>();
	pinkTank->AddComponent<dae::VelocityComponent>(120.f);
	pinkTank->AddComponent<dae::CollisionComponent>(glm::vec2{ 29.f,29.f }, dae::GameCollisionLayers::Tank);
	pinkTank->AddComponent<dae::RenderComponent>(pinkTankTexture);
	pinkTank->AddComponent<dae::TankComponent>(pinkTankTexture->GetSize());
	pinkTank->AddComponent<dae::ShooterComponent>(ObjectType::EnemyBullet, true);

	auto* pinkTankPtr = pinkTank.get();

	scene.Add(std::move(pinkTank));

	//Attach turret to Player Tank
		turretPtr->SetParent(pinkTankPtr, false);
	turretPtr->GetTransform()->SetLocalPosition(
		(tankSize.x - turretSize.x) / 2.f,
		(tankSize.y - turretSize.y) / 2.f);

	return { pinkTankPtr, turretPtr };
}
