#include "TransformComponent.h"
#include "GameObject.h" 

void dae::TransformComponent::SetWorldPosition(float x, float y)
{
    // Convert world to local using current parent if existing
    const auto* parent = GetOwner().GetParent();

    if (parent)
    {
        auto* parentTr = parent->GetTransform();
        const float px = parentTr->GetWorldX();
        const float py = parentTr->GetWorldY();
        m_localX = x - px;
        m_localY = y - py;
    }
    else
    {
        m_localX = x;
        m_localY = y;
    }

    MarkDirty();
}

void dae::TransformComponent::MarkDirty() const
{
    m_dirty = true;     // Mark all children as dirty
    const auto& children = GetOwner().GetChildren();
    for (auto* child : children)
    {
        if (!child) continue;
        if (auto* tr = child->GetTransform())
            tr->RequiresUpdate(); // calls MarkDirty on child
    }
}

void dae::TransformComponent::UpdateWorldIfDirty() const
{
    if (!m_dirty) return;

    const auto* parent = GetOwner().GetParent();
    if (parent)
    {
        auto* parentTr = parent->GetTransform();
        m_worldX = parentTr->GetWorldX() + m_localX;
        m_worldY = parentTr->GetWorldY() + m_localY;
    }
    else
    {
        m_worldX = m_localX;
        m_worldY = m_localY;
    }

    m_dirty = false;
}