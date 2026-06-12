#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include <glm/glm.hpp>

dae::GameObject::GameObject()
{
	m_TransformPtr = &AddComponent<dae::TransformComponent>();
}

dae::GameObject::~GameObject() = default;
// Safely removes components that were queued for deletion during Update()
void dae::GameObject::ProcessPendingRemovals()
{
	if (m_PendingRemovals.empty())
		return;

	std::sort(m_PendingRemovals.begin(), m_PendingRemovals.end(), std::greater<size_t>());

	for (size_t index : m_PendingRemovals)
	{
		if (index < m_components.size())
		{
			m_components.erase(m_components.begin() + index);
		}
	}
	m_PendingRemovals.clear();
}
// Updates all components attached to this GameObject, processes queued component removals, then updates its children
void dae::GameObject::Update(float dt )
{
	for (auto& c : m_components)
		c->Update(dt);

	ProcessPendingRemovals();

	for (auto* child : m_children)
		if (child && !child->IsDestroyed())
			child->Update(dt);
}
// fixed timestep version of Update
void dae::GameObject::FixedUpdate(float fixedDt)
{
	for (auto& c : m_components)
		c->FixedUpdate(fixedDt);

	ProcessPendingRemovals();

	for (auto* child : m_children)
		if (child && !child->IsDestroyed())
			child->FixedUpdate(fixedDt);
}
// Renders all components attached to this GameObject and its children
void dae::GameObject::Render() const
{
	for (const auto& c : m_components)
		c->Render();

	for (auto* child : m_children)
		if (child && !child->IsDestroyed())
			child->Render();
}

void dae::GameObject::SetPosition(float x, float y)
{
	SetPosition(glm::vec2(x, y));
}
void dae::GameObject::SetPosition(glm::vec2 vectorPos)
{
	if (m_TransformPtr)
		m_TransformPtr->SetPosition(vectorPos);
}
// Checks if the child is already in the list before adding to prevent duplicates, used by SetParent to maintain parent-child relationships
void dae::GameObject::AddChild_Internally(GameObject* child)
{
	if (!child) return;
	if (std::find(m_children.begin(), m_children.end(), child) == m_children.end())
		m_children.push_back(child);
}
// Checks if the child is in the list before removing, used by SetParent to maintain parent-child relationships
void dae::GameObject::RemoveChild_Internally(GameObject* child)
{
	auto it = std::find(m_children.begin(), m_children.end(), child);
	if (it != m_children.end())
		m_children.erase(it);
}
// Sets a new parent for this GameObject, optionally keeping the world position unchanged. Prevents cycles in the hierarchy.
void dae::GameObject::SetParent(GameObject* newParent, bool keepWorld)
{
	if (newParent == this) return;

	// Prevent cycles
	for (auto* p = newParent; p != nullptr; p = p->m_parent)
		if (p == this) return;

	float oldX = 0.f, oldY = 0.f;
	if (keepWorld && m_TransformPtr)
	{
		oldX = m_TransformPtr->GetWorldX();
		oldY = m_TransformPtr->GetWorldY();
	}

	// detach from old parent
	if (m_parent)
		m_parent->RemoveChild_Internally(this);

	m_parent = newParent;

	// attach to new parent
	if (m_parent)
		m_parent->AddChild_Internally(this);

	// parent changed => world depends on a different chain now
	if (m_TransformPtr)
		m_TransformPtr->RequiresUpdate();

	if (keepWorld && m_TransformPtr)
		m_TransformPtr->SetWorldPosition(oldX, oldY);
}
// marks GameObject and all its children as destroyed, they will be removed from the scene at the end of the current update loop. This is a recursive function that ensures the entire subtree is marked for destruction
void dae::GameObject::Destroy()
{
	m_IsDestroyed = true;
	for (auto* child : m_children)
		if (child) child->Destroy();
}