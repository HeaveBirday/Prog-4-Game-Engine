#include "WallCollisionComponent.h"
#include "ObjectTypeComponent.h"
#include "TankComponent.h"
#include <Components/VelocityComponent.h>
#include "../Component/EnemyAIComponent.h"
#include <SDL3/SDL_log.h>

void dae::WallCollisionComponent::Update(float )
{
	m_PreviousPos = { GetOwner().GetTransform()->GetX(), GetOwner().GetTransform()->GetY() };
}
// This component listens for collision events and checks if the owner has collided with a wall. 
// If so, it resets the owner's position to the previous frame's position and stops its movement.
void dae::WallCollisionComponent::OnEvent(const Event& event)
{
	if (event.id != EventIds::Collision) return;
	// Check if the owner is involved in the collision
	if (event.objectA != &GetOwner() && event.objectB != &GetOwner()) return;

	// Determine the other object involved in the collision
	GameObject* otherObject =
		event.objectA == &GetOwner() ? event.objectB : event.objectA;

	auto otherType = otherObject->GetComponent<ObjectTypeComponent>();
	if (!otherType) return;
	if (otherType->GetType() == ObjectType::Wall)
	{
		SDL_Log("WallCollisionComponent: hit wall");
	}
	//Check if the collision is with a wall, if not we don't care about it
	if (otherType->GetType() != ObjectType::Wall) return;
	auto* transform = GetOwner().GetTransform();

	transform->SetPosition(m_PreviousPos);


	if (auto* velocity = GetOwner().GetComponent<dae::VelocityComponent>())
	{
		transform->SetPosition(m_PreviousPos);
		velocity->SetDirection({ 0.f, 0.f });
	}


}
