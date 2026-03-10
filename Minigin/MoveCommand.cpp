#include "MoveCommand.h"
#include "Components/TransformComponent.h"

void MoveCommand::Execute()
{
    auto* transform = m_pGameObject->GetTransform();

    float x = transform->GetX();
    float y = transform->GetY();

    switch (m_Direction)
    {
    case Direction::Up:
        y -= m_Speed;
        break;

    case Direction::Down:
        y += m_Speed;
        break;

    case Direction::Left:
        x -= m_Speed;
        break;

    case Direction::Right:
        x += m_Speed;
        break;
    }

    m_pGameObject->SetPosition(x, y);
}