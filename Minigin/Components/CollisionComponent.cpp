#include "CollisionComponent.h"
#include "GameObject.h"
#include <SDL3/SDL_log.h>
#include <EventManager.h>

// Collision detection is done in the Update function of each CollisionComponent. 
// Each component checks for collisions against all other components, but only if its own pointer is smaller than the other component's pointer. 
// This way, we avoid checking the same pair of components twice, since if A checks against B, then B will skip checking against A.
void dae::CollisionComponent::Update(float)
{
	// Check for collisions against all other colliders
	for (CollisionComponent* collider : s_AllColliders)
	{
		if (collider == this)
			continue;
		// Avoid checking the same pair twice.
		if (this > collider)
			continue;
		if (CheckCollision(*collider))
		{
			EventManager::GetInstance().QueueEvent(
				Event{
					dae::EventIds::Collision,
					-1,
					0,
					&GetOwner(),
					&collider->GetOwner()
				});
		}
	}
}

bool dae::CollisionComponent::CheckCollision(const CollisionComponent& otherComponent) const
{
	glm::vec4 cachedRect = GetBoundingBox();
	glm::vec4 otherRect = otherComponent.GetBoundingBox();

	if (cachedRect.x < otherRect.x + otherRect.z &&
		cachedRect.x + cachedRect.z > otherRect.x &&
		cachedRect.y < otherRect.y + otherRect.w &&
		cachedRect.y + cachedRect.w > otherRect.y)
	{
		return true;
	}
	//z is width, W is height

	return false;
}
// The bounding box is calculated by taking the local collision rectangle and adding the world position of the GameObject to it.
glm::vec4 dae::CollisionComponent::GetBoundingBox() const
{
	return { m_CollisionRect.x + GetOwner().GetTransform()->GetX(), m_CollisionRect.y + GetOwner().GetTransform()->GetY(),
								m_CollisionRect.z, m_CollisionRect.w };

}

