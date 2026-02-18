#include "WiggleComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"

void dae::WiggleComponent::Update(float dt)
{
    auto* transform = GetOwner().GetComponent<TransformComponent>();
    if (!transform) return;

    // Move left/right between x=300 and x=420 (simple ping-pong)
    float x = transform->GetX();
    x += m_dir * m_speed * dt;

    if (x > 420.f) { x = 420.f; m_dir = -1.f; }
    if (x < 300.f) { x = 300.f; m_dir = 1.f; }

    transform->SetPosition(x, transform->GetY());
}