#pragma once
#include <glm/glm.hpp>
#include <IEventListener.h>
#include <EventManager.h>
#include <Components/Component.h>
#include <GameObject.h>
namespace dae
{
	// WallCollisionComponent is responsible for handling the collision of a GameObject with walls in the game.
	// It listens for collision events and, when a collision with a wall is detected,
	// it resets the position of the GameObject to its previous position before the collision occurred.
	// This ensures that the GameObject does not pass through walls and maintains proper collision response.
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