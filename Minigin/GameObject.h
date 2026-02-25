#pragma once
#include <string>
#include <memory>
#include "Component.h"
#include "TransformComponent.h"
#include <algorithm>
#include <vector>
#include <type_traits>
#include <cassert>

namespace dae
{
	class Texture2D;
	class GameObject final
	{
	private:
		std::shared_ptr<Texture2D> m_texture{};
		bool m_IsDestroyed{false};

		TransformComponent* m_TransformPtr{ nullptr };

		std::vector<std::unique_ptr<Component>> m_components;
		std::vector<size_t> m_PendingRemovals{};
		void ProcessPendingRemovals();

		GameObject* m_parent{ nullptr };
		std::vector<GameObject*> m_children{};
		void AddChild_Internally(GameObject* child);
		void RemoveChild_Internally(GameObject* child);
	public:

		TransformComponent* GetTransform() const { return m_TransformPtr; }
		void SetPosition(float x, float y);

		void Update(float dt);
		void FixedUpdate(float fixedDt);
		void Render() const;
		
		void SetTexture(const std::string& filename);

		bool IsDestroyed() const { return m_IsDestroyed; }


		bool HasParent() const { return m_parent != nullptr; }
		GameObject* GetParent() const { return m_parent; }
		const std::vector<GameObject*>& GetChildren() const { return m_children; }

		void SetParent(GameObject* newParent, bool keepWorld = true);
		void DetachFromParent(bool keepWorld = true) { SetParent(nullptr, keepWorld); }

		void Destroy();

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args);

		template<typename T>
		T* GetComponent() const;

		template<typename T>
		bool HasComponent() const;

		template<typename T>
		bool RemoveComponent();
		

		GameObject();
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

		if constexpr (std::is_same_v<T, TransformComponent>)
		{
			// Only one transform per GameObject
			assert(m_TransformPtr == nullptr && "GameObject already has a TransformComponent");
		}

		auto component = std::make_unique<T>(std::forward<Args>(args)...);
		component->SetOwner(this);

		T& ref = *component;
		m_components.emplace_back(std::move(component));

		if constexpr (std::is_same_v<T, TransformComponent>)
		{
			m_TransformPtr = &ref;
		}
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
