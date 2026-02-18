#include "SceneManager.h"
#include "Scene.h"


void dae::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

dae::Scene& dae::SceneManager::CreateScene()
{
	m_scenes.emplace_back(new Scene());
	return *m_scenes.back();
}

void dae::SceneManager::Update(float dt)
{
	for(auto& scene : m_scenes)
	{
		scene->Update(dt);
	}
}
void dae::SceneManager::FixedUpdate(float fixedDt)
{
	for (auto& scene : m_scenes)
	{
		scene->FixedUpdate(fixedDt);
	}
}
