#include <algorithm>
#include "Scene.h"

using namespace dae;

void Scene::Add(std::unique_ptr<GameObject> object)
{
	assert(object != nullptr && "Cannot add a null GameObject to the scene.");
	m_PendingObjects.emplace_back(std::move(object));
}

void Scene::Remove(const GameObject& object)
{

	for (auto& obj: m_Objects)
	{
		if (obj.get() == &object)
		{
			obj->Destroy();
			break;
		}
	}
}

void Scene::RemoveAll()
{
	m_Objects.clear();
	m_PendingObjects.clear();
}

void dae::Scene::FixedUpdate(float fixedDt)
{
	ProcessPendingObjects();

	for (auto& object : m_Objects)
	{
		if (!object->HasParent())
			object->FixedUpdate(fixedDt);
	}

	RemoveDestroyedObjects();
	ProcessPendingObjects();
}

void Scene::Update(float dt)
{
	ProcessPendingObjects();

	for(auto& object : m_Objects)
	{
		if (!object->HasParent())
			object->Update(dt);
	}
	RemoveDestroyedObjects();
	ProcessPendingObjects();
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		if (!object->HasParent())
			object->Render();
	}
		
}

void dae::Scene::ProcessPendingObjects()
{
	for (auto& object : m_PendingObjects)
	{
		m_Objects.emplace_back(std::move(object));
	}

	m_PendingObjects.clear();
}

void dae::Scene::RemoveDestroyedObjects()
{
	std::erase_if(m_Objects, [](const std::unique_ptr<GameObject>& obj)
		{
			return obj->IsDestroyed();
		});
}

