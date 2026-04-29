#include "CollisionComponent.h"
#include "GameObject.h"
#include <SDL3/SDL_log.h>

void dae::CollisionComponent::Update(float)
{
	//TODO Order dependency is happening here, so figure out a way to make sure that all collision components are updated before checking for collisions. 
	//Maybe add a separate collision system that runs after all updates,
	//or add a flag to check if the component has been updated this frame and only check for collisions after all components have been updated.
	//EASIEST FIX WOULD BE TO ADD A LATE UPDATE FUNCTION THAT IS CALLED AFTER ALL UPDATES, AND CHECK FOR COLLISIONS THERE. IF I DO THE LATE UPDATE, WHAT I WANT TO DO IS
	//THE MOVEMENT UPDATE IN THE LATE UPDATE, SO THAT ALL COLLISION COMPONENTS HAVE THEIR NEW POSITIONS BEFORE CHECKING FOR COLLISIONS. THEN I CAN ALSO ADD A SEPARATE LATE UPDATE FUNCTION FOR 
	//OTHER COMPONENTS THAT NEED TO RUN AFTER COLLISIONS ARE CHECKED, LIKE HEALTH COMPONENTS OR SOMETHING.


	//add observer pattern, so that when a collision is detected, an event is fired and the event manager can notify listeners.
	for (CollisionComponent* collider : s_AllColliders)
	{
		if (collider == this)
			continue;
		if (CheckCollision(*collider))
		{
			SDL_Log("Collision detected between two objects");
			//Fire collision event here, with the two game objects as parameters
			//EventManager::GetInstance().QueueEvent(CollisionEvent{ GetOwner(), other->GetOwner() });
		}
		else
		{
			SDL_Log("No Collision");
		}
		//broadcast it as a subject not a whole event manager, limited to just game object that there was a collision
		// OPTION: My component gets notified as a method that it was collided with and then it can handle it there,
		// or it can fire an event that other components can listen to and handle it there.
	}
}
//If the player collides with a wall vs another tank or a tank collides with a wall how do i deferientate between the two? 
// Maybe add a tag component to the game object and check for the tag in the collision event, so that I can differentiate between
// different types of collisions and handle them accordingly. -> HOW?

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

