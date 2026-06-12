#pragma once
#include <Components/Component.h>
#include <IEventListener.h>
#include "EventManager.h"

namespace dae
{
	// PlayerTouchHitComponent listens for collision events and checks if the player has collided with a specific tile type (e.g., a teleport tile).
	// If such a collision is detected, it triggers a teleportation action for the player. 
	// The component uses the level data to determine the tile type at the player's position and reacts accordingly.
	class PlayerTouchHitComponent final : public Component, public IEventListener
	{
	public:
		explicit PlayerTouchHitComponent(GameObject* owner, const std::vector<std::string>& level)
			: Component(owner), m_Level(level)
		{
			EventManager::GetInstance().AddListener(this);
		}
		~PlayerTouchHitComponent() override
		{
			EventManager::GetInstance().RemoveListener(this);
		}

		void OnEvent(const Event& event) override;
	private:
		void TeleportPlayer();
		const std::vector<std::string>& m_Level;
		float m_TileSize{ 32.f };

	};
}