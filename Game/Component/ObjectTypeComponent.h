#pragma once
#include <Components/Component.h>

enum class ObjectType
{
    Player,
    EnemyTank,
    Recognizer,
    PlayerBullet,
    EnemyBullet,
    Wall,
    Diamond
};
class ObjectTypeComponent final : public dae::Component
{
public:
	ObjectTypeComponent(dae::GameObject* owner, ObjectType type)
        : Component(owner), m_Type(type) {}
	ObjectType GetType() const { return m_Type; }

private:
	ObjectType m_Type;
};