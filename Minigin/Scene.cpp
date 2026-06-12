#include <algorithm>
#include "Scene.h"

using namespace dae;
// The Scene class manages GameObjects, allowing adding, removing, updating, and rendering them. It also handles pending additions and removals to ensure safe modifications during iteration
// The Add method adds a new GameObject to the scene, storing it in a pending list to be processed later. 
void Scene::Add(std::unique_ptr<GameObject> object)
{
	assert(object != nullptr && "Cannot add a null GameObject to the scene.");
	m_PendingObjects.emplace_back(std::move(object));
}
// The Remove method marks a GameObject for destruction, and RemoveAll clears all GameObjects from the scene
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
//The FixedUpdate method process pending additions, update all GameObjects, and then remove any that have been marked for destruction.
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
// The Update method follows the same pattern as FixedUpdate but calls the regular Update method on each GameObject instead.
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
// The Render method iterates through all GameObjects and calls their Render method, but only for those that do not have a parent, ensuring that child objects are rendered as part of their parent.
void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		if (!object->HasParent())
			object->Render();
	}
		
}
// The ProcessPendingObjects method moves all GameObjects from the pending list to the main list, allowing them to be updated and rendered in subsequent frames.
void dae::Scene::ProcessPendingObjects()
{
	for (auto& object : m_PendingObjects)
	{
		m_Objects.emplace_back(std::move(object));
	}

	m_PendingObjects.clear();
}
// The RemoveDestroyedObjects method removes any GameObjects that have been marked as destroyed from the main list.
void dae::Scene::RemoveDestroyedObjects()
{
	std::erase_if(m_Objects, [](const std::unique_ptr<GameObject>& obj)
		{
			return obj->IsDestroyed();
		});
}

