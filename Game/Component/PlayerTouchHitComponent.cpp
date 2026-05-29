#include "PlayerTouchHitComponent.h"
#include "ObjectTypeComponent.h"
#include <GameObject.h>
#include <IEventListener.h>
#include <Components/Component.h>
#include "../TronEvents.h"
void dae::PlayerTouchHitComponent::OnEvent(const Event& event)
{
	if (event.id != EventIds::Collision) return;
	// Check if the owner is involved in the collision
	if (event.objectA != &GetOwner() && event.objectB != &GetOwner()) return;

	// Determine the other object involved in the collision
	GameObject* otherObject =
		event.objectA == &GetOwner() ? event.objectB : event.objectA;

	auto otherType = otherObject->GetComponent<ObjectTypeComponent>();
	if (!otherType) return;


	if (otherType->GetType() == ObjectType::Diamond)
	{
		TeleportPlayer();
		return;
	}

	if (otherType->GetType() == ObjectType::Recognizer || otherType->GetType() == ObjectType::EnemyTank)
	{
		EventManager::GetInstance().QueueEvent(dae::Event{
		TronEventIds::PlayerHit,
		-1,
		1,
		nullptr,
		nullptr
			});
	}
	

}

void dae::PlayerTouchHitComponent::TeleportPlayer()
{
	std::vector<glm::ivec2> walkableTiles;

	for (int y = 0; y < static_cast<int>(m_Level.size()); ++y)
	{
		for (int x = 0; x < static_cast<int>(m_Level[y].size()); ++x)
		{
			if (m_Level[y][x] == '_')
			{
				walkableTiles.push_back({ x, y });
			}
		}
	}

	if (walkableTiles.empty())
		return;

	const int index = rand() % static_cast<int>(walkableTiles.size());
	glm::ivec2 tile = walkableTiles[index];

	GetOwner().SetPosition({
		tile.x * m_TileSize,
		tile.y * m_TileSize
		});
}
