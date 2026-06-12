#include "RotateTurretCommand.h"

// just rotates it and sets the rotation to match the turret rotation so bullets fly in the right direction (since Shooting can use the turret direction instead of the forward direction)
void RotateTurretCommand::Execute(float dt)
{
	m_TankComponent.RotateTurret(m_RotationSpeed * dt);

	m_TurretObject.GetTransform()->SetRotation(m_TankComponent.GetTurretAngle());
}
