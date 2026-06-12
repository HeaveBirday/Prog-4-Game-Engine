#pragma once
#include <string>
#include <memory>
#include "Components/Component.h"
#include "Components/TransformComponent.h"
#include <algorithm>
#include <vector>
#include <type_traits>
#include <cassert>
#include <concepts>

namespace dae
{
	class Texture2D;
	class GameObject final
	{
	private:
		bool m_IsDestroyed{false};
		//Cached pointer to the mandatory TransformComponent for quick access. Since every GameObject must have one, we can safely assume it exists after construction.

		TransformComponent* m_TransformPtr{ nullptr };
		// Components are stored as unique_ptrs because the GameObject owns them
		std::vector<std::unique_ptr<Component>> m_components;
		// Components are removed after Update/FixedUpdate to avoid erasing while iterating
		std::vector<size_t> m_PendingRemovals{};
		void ProcessPendingRemovals();

		// Raw pointers are used here because the Scene owns the GameObjects, the parent-child system only links objects together
		GameObject* m_parent{ nullptr };
		std::vector<GameObject*> m_children{};
		void AddChild_Internally(GameObject* child);
		void RemoveChild_Internally(GameObject* child);
	public:

		TransformComponent* GetTransform() const { return m_TransformPtr; }
		void SetPosition(float x, float y);
		void SetPosition(glm::vec2 vectorPos);

		void Update(float dt);
		void FixedUpdate(float fixedDt);
		void Render() const;
		

		bool IsDestroyed() const { return m_IsDestroyed; }


		bool HasParent() const { return m_parent != nullptr; }
		GameObject* GetParent() const { return m_parent; }
		const std::vector<GameObject*>& GetChildren() const { return m_children; }

		void SetParent(GameObject* newParent, bool keepWorld = true);
		void DetachFromParent(bool keepWorld = true) { SetParent(nullptr, keepWorld); }

		void Destroy();

		template<std::derived_from<Component> T, typename... Args> requires std::constructible_from<T, GameObject*, Args...>
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

	
	// Creates a new component, assigns this GameObject as its owner,stores it in the component list, and returns a reference to it. Only one TransformComponent can be added per GameObject.
	template<std::derived_from<Component> T, typename... Args> requires std::constructible_from<T, GameObject*, Args...>
	T& GameObject::AddComponent(Args&&... args)
	{
		static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

		if constexpr (std::is_same_v<T, TransformComponent>)
		{
			// Only one transform per GameObject
			assert(m_TransformPtr == nullptr && "GameObject already has a TransformComponent");
		}

		auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
	

		T& ref = *component;
		m_components.emplace_back(std::move(component));

		if constexpr (std::is_same_v<T, TransformComponent>)
		{
			m_TransformPtr = &ref;
		}
		return ref;
	}
	// Searches for and returns the first component of the requested type, returns nullptr if the GameObject does not contain that component
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
	// Checks whether this GameObject contains a component of the requested type
	template<typename T>
	bool GameObject::HasComponent() const
	{
		return GetComponent<T>() != nullptr;
	}

	// Marks a component for removal, actual removal happens after the update loop to avoid iterator invalidation
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
