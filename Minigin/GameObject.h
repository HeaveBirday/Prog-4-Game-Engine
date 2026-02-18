#pragma once
#include <string>
#include <memory>
#include "Transform.h"
#include "Component.h"
#include <algorithm>
#include <vector>
#include <type_traits>

namespace dae
{
	class Texture2D;
	class GameObject final
	{
	private:

		Transform m_transform{};
		std::shared_ptr<Texture2D> m_texture{};
		bool m_IsDestroyed{false};

		std::vector<std::unique_ptr<Component>> m_components;
		std::vector<size_t> m_PendingRemovals{};
		void ProcessPendingRemovals();
	public:

		void Update(float dt);
		void FixedUpdate(float fixedDt);
		void Render() const;
		
		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		void Destroy() { m_IsDestroyed = true; }
		bool IsDestroyed() const { return m_IsDestroyed; };

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args);

		template<typename T>
		T* GetComponent() const;

		template<typename T>
		bool HasComponent() const;

		template<typename T>
		bool RemoveComponent();
		

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	};
	template<typename T, typename... Args>
	T& GameObject::AddComponent(Args&&... args)
	{
		static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

		auto component = std::make_unique<T>(std::forward<Args>(args)...);
		component->SetOwner(this);

		T& ref = *component;
		m_components.emplace_back(std::move(component));
		return ref;
	}
	template<typename T>
	T* GameObject::GetComponent() const
	{
		static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

		for (const auto& component : m_components)
		{
			if (auto casted = dynamic_cast<T*>(component.get()))
				return casted;
		}
		return nullptr;
	}

	template<typename T>
	bool GameObject::HasComponent() const
	{
		return GetComponent<T>() != nullptr;
	}

	template<typename T>
	bool GameObject::RemoveComponent()
	{
		static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

		for (size_t idx = 0; idx < m_components.size(); ++idx)
		{
			if (dynamic_cast<T*>(m_components[idx].get()))
			{
				// SAFE removal: queue it, don't erase during Update loop
				m_PendingRemovals.push_back(idx);
				return true;
			}
		}
		return false;
	}
}
