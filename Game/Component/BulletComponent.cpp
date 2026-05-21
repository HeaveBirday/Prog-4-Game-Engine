#include "BulletComponent.h"

void dae::BulletComponent::Update(float )
{

}

void dae::BulletComponent::OnEvent(const Event& event)
{
	if (event.id != EventIds::Collision) return;

	if (event.objectA != &GetOwner() && event.objectB != &GetOwner()) return;

	GetOwner().Destroy();
}
