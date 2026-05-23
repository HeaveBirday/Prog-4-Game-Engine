#include "WallCollisionComponent.h"
#include "ObjectTypeComponent.h"
#include "TankComponent.h"
void dae::WallCollisionComponent::Update(float )
{
	m_PreviousPos = { GetOwner().GetTransform()->GetX(), GetOwner().GetTransform()->GetY() };
}

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
	//Check if the collision is with a wall, if not we don't care about it
	if (otherType->GetType() != ObjectType::Wall) return;
	int blergh{};
	blergh++;
	//no further checks needed since only GameObjects that should be blocked by the wall have the WallCollisionComponent IE tanks
	GetOwner().GetTransform()->SetPosition(m_PreviousPos);
	

}
