#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace dae
{
	class VelocityComponent final : public Component
	{
    public:
        explicit VelocityComponent(GameObject* owner, float speed)
			:Component(owner), m_Speed(speed), m_Direction()
        {
        }

        void Update(float dt) override;

        void SetDirection(glm::vec2 dir) { m_Direction = glm::normalize(dir); }

    private:
        float m_Speed;
		glm::vec2 m_Direction;
	};

}