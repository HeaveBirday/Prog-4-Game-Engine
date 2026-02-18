#pragma once
#include "Component.h"

namespace dae
{
    class TextComponent;

    class FPSComponent final : public Component
    {
    public:
        explicit FPSComponent(float refreshInterval = 0.25f)
            : m_refreshInterval(refreshInterval)
        {
        }

        void Update(float dt) override;

    private:
        float m_refreshInterval{ 0.25f };
        float m_accum{ 0.f };
        int m_frames{ 0 };
    };
}