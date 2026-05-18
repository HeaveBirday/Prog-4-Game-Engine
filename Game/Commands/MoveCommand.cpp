#include "MoveCommand.h"
#include "Components/TransformComponent.h"
#include "GameObject.h"

void MoveCommand::Execute(float)
{

	m_VelocityComponent.SetDirection(m_MoveDirection);
	
	if (m_MoveDirection != glm::vec2{ 0.f,0.f })
	{
		if (m_MoveDirection == glm::vec2{ 0.f, -1.f })
		{
			m_TankComponent.SetDirection(dae::Direction::Up);
			m_TankComponent.GetOwner().GetTransform()->SetRotation(270.f);

		}		
		else if (m_MoveDirection == glm::vec2{ 0.f, 1.f })
		{
			m_TankComponent.SetDirection(dae::Direction::Down);
			m_TankComponent.GetOwner().GetTransform()->SetRotation(90.f);
		}
			

		else if (m_MoveDirection == glm::vec2{ -1.f, 0.f })
		{
			m_TankComponent.SetDirection(dae::Direction::Left);
			m_TankComponent.GetOwner().GetTransform()->SetRotation(180.f);

		}
		else if (m_MoveDirection == glm::vec2{ 1.f, 0.f })
		{
			m_TankComponent.SetDirection(dae::Direction::Right);
			m_TankComponent.GetOwner().GetTransform()->SetRotation(0.f);

		}
	}
	
}
