#include "TransformComponent.h"
#include "TransformComponent.h"
#include "GameObject.h" 


void dae::TransformComponent::SetWorldPosition(float x, float y)
{
	SetWorldPosition(glm::vec2(x, y));
}
void dae::TransformComponent::SetWorldPosition(glm::vec2 pos)
{
    // Convert world to local using current parent if existing
    const auto* parent = GetOwner().GetParent();

    if (parent)
    {
        auto* parentTr = parent->GetTransform();

        const glm::vec2 parentPos = parentTr->GetWorldPosition();
       

        m_LocalPos = parentPos;
    }
    else
    {
		m_LocalPos = pos;
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
        m_WorldPos = parentTr->GetWorldPosition() + m_LocalPos;
    
    }
    else
    {
        m_WorldPos = m_LocalPos;
    }

    m_dirty = false;
}