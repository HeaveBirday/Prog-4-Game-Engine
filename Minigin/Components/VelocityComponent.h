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

        void SetDirection(glm::vec2 dir) 
        {
			if (dir == glm::vec2{ 0.f, 0.f })
            {
                m_Direction = dir;
                return;
            }
            m_Direction = glm::normalize(dir); 
        }
        glm::vec2 GetDirection() const
        {
            return m_Direction;
        }
    private:
        float m_Speed;
		glm::vec2 m_Direction;
	};

}