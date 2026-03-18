#include "LivesDisplayComponent.h"
#include "TextComponent.h"
#include "../EventManager.h"
#include "../GameObject.h"
#include <string>

dae::LivesDisplayComponent::LivesDisplayComponent(GameObject* owner, int playerId)
    : Component(owner)
    , m_PlayerId(playerId)
{
    EventManager::GetInstance().AddListener(this);
    UpdateText();
}

dae::LivesDisplayComponent::~LivesDisplayComponent()
{
    EventManager::GetInstance().RemoveListener(this);
}

void dae::LivesDisplayComponent::OnEvent(const Event& event)
{
    if (event.playerId != m_PlayerId)
        return;

    if (event.type == EventType::LivesChanged)
    {
        m_Lives = event.value;
        UpdateText();
    }
}

void dae::LivesDisplayComponent::UpdateText()
{
    auto* text = GetOwner().GetComponent<dae::TextComponent>();
    if (text)
    {
        text->SetText("Lives: " + std::to_string(m_Lives));
    }
}