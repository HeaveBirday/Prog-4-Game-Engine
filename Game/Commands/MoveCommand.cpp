#include "MoveCommand.h"
#include "Components/TransformComponent.h"
#include "GameObject.h"

void MoveCommand::Execute(float)
{

	m_VelocityComponent.SetDirection(m_MoveDirection);
	
	m_TankComponent.SetMoveDirection(m_MoveDirection);
	
}
