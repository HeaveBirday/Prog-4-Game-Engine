#pragma once
#include <Commands/Command.h>
#include "../Component/TankComponent.h"
#include <GameObject.h>

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