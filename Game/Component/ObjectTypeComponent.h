#pragma once
#include <Components/Component.h>

// ObjectTypeComponent is a simple component that holds the type of the GameObject,
// allowing for easy identification and categorization of objects in the game. 
// This can be useful for collision handling, rendering decisions, or any logic that depends on the type of object.
enum class ObjectType
{
    Player,
    EnemyTank,
    Recognizer,
    PlayerBullet,
    EnemyBullet,
    Wall,
    Diamond,

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