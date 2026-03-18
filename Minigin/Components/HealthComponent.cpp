#include "HealthComponent.h"
#include "../EventManager.h"
#include "../Event.h"

dae::HealthComponent::HealthComponent(GameObject* owner, int playerId, int startLives)
    : Component(owner)
    , m_PlayerId(playerId)
    , m_Lives(startLives)
{
}

void dae::HealthComponent::LoseLife()
{
    if (m_Lives <= 0)
        return;

    --m_Lives;

    EventManager::GetInstance().QueueEvent(
        Event{ EventType::LivesChanged, m_PlayerId, m_Lives }
    );

    if (m_Lives <= 0)
    {
        EventManager::GetInstance().QueueEvent(
            Event{ EventType::PlayerDied, m_PlayerId, 0 }
        );
    }
}