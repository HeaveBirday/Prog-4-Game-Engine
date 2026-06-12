#pragma once
#include <Commands/Command.h>
#include "../Component/TankComponent.h"
#include <GameObject.h>

// RotateTurretCommand rotates the turret of a tank at a specified speed.
// It is executed every frame while the corresponding input is active, allowing for smooth turret rotation based on player input.
class RotateTurretCommand final : public Command
{
public:
	RotateTurretCommand(dae::TankComponent& tankComponent, dae::GameObject& turretObject, float rotationSpeed):
		m_TankComponent(tankComponent), m_TurretObject(turretObject), m_RotationSpeed(rotationSpeed)
	{
	
	}
	void Execute(float dt) override;
private:
	dae::TankComponent& m_TankComponent;
	dae::GameObject& m_TurretObject;
	float m_RotationSpeed{};
};