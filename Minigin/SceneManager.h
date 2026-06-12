#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Scene.h"
#include "Singleton.h"

namespace dae
{
	class Scene;
	// The SceneManager is responsible for managing multiple scenes in the game. It allows for creating new scenes, retrieving the active scene, updating and rendering all scenes, and removing all scenes when necessary.
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene();
		Scene& GetActiveScene();

		void FixedUpdate(float fixedDt);
		void Update(float dt);
		void Render();
		void RemoveAll();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::unique_ptr<Scene>> m_scenes{};
	};
}
