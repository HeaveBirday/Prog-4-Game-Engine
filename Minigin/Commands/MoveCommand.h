#pragma once
#include "Command.h"
#include "GameObject.h"


class MoveCommand final : public Command
{
public:
enum class Direction
{
    Up,
    Down,
    Left,
    Right
};
    MoveCommand(dae::GameObject* gameObject, Direction direction, float speed)
        : m_pGameObject(gameObject),
		m_Direction(direction),
		m_Speed(speed)
    {
    }
   
    void Execute() override;

private:
    dae::GameObject* m_pGameObject{};
    Direction m_Direction{};
    float m_Speed;
};