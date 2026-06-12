#pragma once
#include <Components/Component.h>
#include <Components/VelocityComponent.h>
#include <Components/CollisionComponent.h>

#include <GameObject.h>

#include <EventManager.h>
#include <IEventListener.h>
namespace dae
{
	// BulletComponent handles the movement and collision of bullets fired by tanks. 
	// It listens for collision events to determine when it hits an enemy, the player, or a wall,
	// and reacts accordingly (damaging enemies, damaging the player, or bouncing off walls).
	class BulletComponent final : public Component, public IEventListener
	{
	public:
		explicit BulletComponent(GameObject* owner, float speed, glm::vec2 direction)
			:Component(owner),
			m_Speed(speed), 
			m_Direction(direction), 
			m_Transform(owner->GetTransform()), 
			m_Velocity(owner->GetComponent<VelocityComponent>()),
			m_Collision(owner->GetComponent<CollisionComponent>())
		{
			owner->GetComponent<VelocityComponent>()->SetDirection(m_Direction);

			EventManager::GetInstance().AddListener(this);
		}
		~BulletComponent()
		{
			EventManager::GetInstance().RemoveListener(this);
		}

		void Update(float dt) override;

		void OnEvent(const Event& event) override;

		void EnemyHit(GameObject* otherObject);
		void PlayerHit();
		void Bounce(GameObject* wall);

	private:
		float m_Speed;
		glm::vec2 m_Direction{};
		TransformComponent* m_Transform{};
		VelocityComponent* m_Velocity{};
		CollisionComponent* m_Collision{};
		glm::vec2 m_PreviousPos{};
		int m_BounceCount{};
		int m_MaxBounces{ 5 };
		bool m_HasBouncedThisFrame{};
		float m_LifeTime{ 3.f };
	};
}
