#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

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
}

void dae::GameObject::FixedUpdate(float fixedDt)
{
	for (auto& c : m_components)
		c->FixedUpdate(fixedDt);

	ProcessPendingRemovals();
}
//TO DO FIX LATER RENDERER 
void dae::GameObject::Render() const
{
	for (const auto& c : m_components)
		c->Render();
}

void dae::GameObject::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}
