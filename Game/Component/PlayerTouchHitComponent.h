#pragma once
#include <Components/Component.h>
#include <IEventListener.h>
#include "EventManager.h"

namespace dae
{
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