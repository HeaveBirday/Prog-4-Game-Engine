#include "CollisionComponent.h"
#include "GameObject.h"
#include <SDL3/SDL_log.h>
#include <EventManager.h>

void dae::CollisionComponent::Update(float)
{
	//A FIX WOULD BE TO ADD A LATE UPDATE FUNCTION THAT IS CALLED AFTER ALL UPDATES, AND CHECK FOR COLLISIONS THERE. IF I DO THE LATE UPDATE, WHAT I WANT TO DO IS
	//THE MOVEMENT UPDATE IN THE LATE UPDATE, SO THAT ALL COLLISION COMPONENTS HAVE THEIR NEW POSITIONS BEFORE CHECKING FOR COLLISIONS. THEN I CAN ALSO ADD A SEPARATE LATE UPDATE FUNCTION FOR 
	//OTHER COMPONENTS THAT NEED TO RUN AFTER COLLISIONS ARE CHECKED, LIKE HEALTH COMPONENTS OR SOMETHING.

	for (CollisionComponent* collider : s_AllColliders)
	{
		if (collider == this)
			continue;
		if (this > collider)
			continue;
		if (CheckCollision(*collider))
		{
			//SDL_Log("Collision detected between %p and %p", &GetOwner(), &collider->GetOwner());
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

glm::vec4 dae::CollisionComponent::GetBoundingBox() const
{
	return { m_CollisionRect.x + GetOwner().GetTransform()->GetX(), m_CollisionRect.y + GetOwner().GetTransform()->GetY(),
								m_CollisionRect.z, m_CollisionRect.w };

}

