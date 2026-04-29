#pragma once
#include "Command.h"
#include <glm/glm.hpp>
#include "Components/VelocityComponent.h"


class MoveCommand final : public Command
{
public:

    MoveCommand(dae::VelocityComponent& velComponent, glm::vec2 dirVector)
        : m_VelocityComponent(velComponent),
		m_Direction(dirVector)
    {   
    }
   
    void Execute(float deltaTime) override;

private:
    dae::VelocityComponent& m_VelocityComponent;
	glm::vec2 m_Direction;
};