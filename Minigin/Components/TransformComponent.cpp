#include "TransformComponent.h"
#include "GameObject.h" 


void dae::TransformComponent::SetWorldPosition(float x, float y)
{
	SetWorldPosition(glm::vec2(x, y));
}
// converts the given world coordinates to local coordinates based on the current parent transform (if any). This ensures that the local position is correctly updated to reflect the new world position, 
// and that any child objects will maintain their relative positions when the parent moves.
void dae::TransformComponent::SetWorldPosition(glm::vec2 pos)
{
    // Convert world to local using current parent if existing
    const auto* parent = GetOwner().GetParent();

    if (parent)
    {
        auto* parentTr = parent->GetTransform();

        const glm::vec2 parentPos = parentTr->GetWorldPosition();
       

        m_LocalPos = pos - parentPos;
    }
    else
    {
		m_LocalPos = pos;
    }

    MarkDirty();
}

// When a transform is marked dirty, it means its world position needs to be recalculated. This also means all children need to be marked dirty as well, since their world position depends on their parent's world position.
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
// Recalculates world position if dirty. This is called whenever world position data is requested, ensuring that the world position is always up to date when accessed.
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