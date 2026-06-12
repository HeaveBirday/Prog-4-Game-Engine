#pragma once
#include <memory>
#include <string>
#include <vector>
#include "GameObject.h"

namespace dae
{
	// The Scene class manages a collection of GameObjects, providing functionality to add, remove, update, and render them. It also handles pending additions and removals to ensure safe iteration during updates.
	class Scene final
	{
	public:
		void Add(std::unique_ptr<GameObject> object);
		void Remove(const GameObject& object);
		void RemoveAll();

		void FixedUpdate(float fixedDt);
		void Update(float dt);
		void Render() const;

		~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private:
		friend class SceneManager;
		explicit Scene() = default;

		void ProcessPendingObjects();
		void RemoveDestroyedObjects();
		std::vector < std::unique_ptr<GameObject>> m_Objects{};
		std::vector <std::unique_ptr<GameObject>> m_PendingObjects;
	};

}
