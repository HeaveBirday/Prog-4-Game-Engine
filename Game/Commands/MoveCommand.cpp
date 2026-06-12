#include "MoveCommand.h"

void MoveCommand::Execute(float)
{

	m_VelocityComponent.SetDirection(m_MoveDirection);
	
	m_TankComponent.SetMoveDirection(m_MoveDirection);
	
}
