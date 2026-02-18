#pragma once
#include "Component.h"

namespace dae
{
    class TransformComponent final : public Component
    {
    public:
        void SetPosition(float x, float y) { m_xPos = x; m_yPos = y; }
        float GetX() const { return m_xPos; }
        float GetY() const { return m_yPos; }

    private:
        float m_xPos{0.f}, m_yPos{0.f};
    };
}