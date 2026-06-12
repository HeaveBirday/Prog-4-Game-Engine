#include "ShootBulletCommand.h"
#include <GameObject.h>
#include "../Component/ShooterComponent.h"

// tells the owner's ShooterComponent to shoot a bullet.
void dae::ShootBulletCommand::Execute(float)
{

	if (!m_PlayerObject) return;
	auto shooter = m_PlayerObject->GetComponent<ShooterComponent>();

	shooter->Shoot();

	
	
}
