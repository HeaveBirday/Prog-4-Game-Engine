#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::GameObject()
{
	m_TransformPtr = &AddComponent<dae::TransformComponent>();
}

dae::GameObject::~GameObject() = default;

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

void dae::GameObject::Update(float dt )
{
	for (auto& c : m_components)
		c->Update(dt);

	ProcessPendingRemovals();

	for (auto* child : m_children)
		if (child && !child->IsDestroyed())
			child->Update(dt);
}

void dae::GameObject::FixedUpdate(float fixedDt)
{
	for (auto& c : m_components)
		c->FixedUpdate(fixedDt);

	ProcessPendingRemovals();

	for (auto* child : m_children)
		if (child && !child->IsDestroyed())
			child->FixedUpdate(fixedDt);
}
//TODO FIX LATER: RENDERER 
void dae::GameObject::Render() const
{
	for (const auto& c : m_components)
		c->Render();

	for (auto* child : m_children)
		if (child && !child->IsDestroyed())
			child->Render();
}

void dae::GameObject::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::GameObject::SetPosition(float x, float y)
{
	if (m_TransformPtr)
		m_TransformPtr->SetPosition(x, y);
}
void dae::GameObject::AddChild_Internally(GameObject* child)
{
	if (!child) return;
	if (std::find(m_children.begin(), m_children.end(), child) == m_children.end())
		m_children.push_back(child);
}

void dae::GameObject::RemoveChild_Internally(GameObject* child)
{
	auto it = std::find(m_children.begin(), m_children.end(), child);
	if (it != m_children.end())
		m_children.erase(it);
}
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
void dae::GameObject::Destroy()
{
	m_IsDestroyed = true;
	for (auto* child : m_children)
		if (child) child->Destroy();
}