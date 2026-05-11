#pragma once
#include <Components/Component.h>
#include <Components/VelocityComponent.h>
#include <GameObject.h>

namespace dae
{
	class BulletComponent final : public Component
	{
	public:
		explicit BulletComponent(GameObject* owner, float speed, glm::vec2 direction)
			:Component(owner), m_Speed(speed), m_Direction(direction) 
		{
			//make sure to first add the velocity component when creatin the 
			owner->GetComponent<VelocityComponent>()->SetDirection(m_Direction);
		}

		void Update(float dt) override;
	private:
		float m_Speed;
		glm::vec2 m_Direction;

	};
}
