#pragma once
#include "Component.h"

namespace dae
{
    class TransformComponent;

    class WiggleComponent final : public Component
    {
    public:
        explicit WiggleComponent(GameObject* owner, float speed = 50.f) : Component(owner),m_speed(speed) {}

        void Update(float dt) override;

    private:
        float m_speed{ 50.f };
        float m_dir{ 1.f };
    };
}