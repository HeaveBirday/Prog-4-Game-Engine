#include "ShootBulletCommand.h"
#include <GameObject.h>
#include "../Component/ShooterComponent.h"

void dae::ShootBulletCommand::Execute(float)
{

	if (!m_PlayerObject) return;
	auto shooter = m_PlayerObject->GetComponent<ShooterComponent>();

	shooter->Shoot();

	
	
}
