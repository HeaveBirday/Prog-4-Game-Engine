#include "RotateTurretCommand.h"

void RotateTurretCommand::Execute(float dt)
{
	m_TankComponent.RotateTurret(m_RotationSpeed * dt);

	m_TurretObject.GetTransform()->SetRotation(m_TankComponent.GetTurretAngle());
}
