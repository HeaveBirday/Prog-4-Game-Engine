#pragma once
#include "Component.h"

namespace dae
{
    class HealthComponent final : public Component
    {
    public:
        HealthComponent(GameObject* owner, int playerId, int startLives);

        void LoseLife();

        int GetLives() const { return m_Lives; }
        int GetPlayerId() const { return m_PlayerId; }

    private:
        int m_PlayerId{};
        int m_Lives{};
    };
}