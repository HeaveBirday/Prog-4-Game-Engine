#include "MoveCommand.h"
#include "Components/TransformComponent.h"

void MoveCommand::Execute(float)
{


	m_VelocityComponent.SetDirection(m_Direction);
	//auto* transform = m_pGameObject->GetTransform();

	//glm::vec2 pos{ transform->GetX(), transform->GetY() };
	//pos += m_Direction * m_Speed * deltaTime;

	//m_pGameObject->SetPosition(pos.x, pos.y);

	//Instead of moving the gameobject directly through transform, create a velocity component or whatever name. Velocity component has a velocity vector. 
	// Then in the update of the velocity component, move the game object there. So in MoveCommand, instead of passing a tank pointer, i pass a velocity component to the tank

}