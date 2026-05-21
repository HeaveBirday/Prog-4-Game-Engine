#pragma once
#include <Components/Component.h>
#include <Components/VelocityComponent.h>
#include <GameObject.h>

#include <EventManager.h>
#include <IEventListener.h>
namespace dae
{
	class BulletComponent final : public Component, public IEventListener
	{
	public:
		explicit BulletComponent(GameObject* owner, float speed, glm::vec2 direction)
			:Component(owner), m_Speed(speed), m_Direction(direction)
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
		
	private:
		float m_Speed;
		glm::vec2 m_Direction;

	};
}
