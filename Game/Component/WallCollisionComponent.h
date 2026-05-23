#pragma once
#include <glm/glm.hpp>
#include <IEventListener.h>
#include <EventManager.h>
#include <Components/Component.h>
#include <GameObject.h>
namespace dae
{
	class WallCollisionComponent final : public Component, public IEventListener
	{
	public:
		explicit WallCollisionComponent(GameObject* owner) : Component(owner)
		{
			EventManager::GetInstance().AddListener(this);
		}
		~WallCollisionComponent() override
		{
			EventManager::GetInstance().RemoveListener(this);
		}
		void Update(float dt) override;
		void OnEvent(const Event& event) override;

	private:
		glm::vec2 m_PreviousPos{};

	};
}