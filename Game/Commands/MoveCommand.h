#pragma once
#include "Commands/Command.h"
#include <glm/glm.hpp>
#include "Components/VelocityComponent.h"
#include "../Component/TankComponent.h"
#include <Components/RenderComponent.h>
class MoveCommand final : public Command
{
public:

    MoveCommand(dae::VelocityComponent& velComponent,
        dae::TankComponent& tankComponent,
        glm::vec2 dirVector)
        : m_VelocityComponent(velComponent),
		m_TankComponent(tankComponent),
		m_MoveDirection(dirVector)
    {   
    }
   
    void Execute(float deltaTime) override;

private:
    dae::VelocityComponent& m_VelocityComponent;
	dae::TankComponent& m_TankComponent;
	glm::vec2 m_MoveDirection;
};