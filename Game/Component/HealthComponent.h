#pragma once
#include <Components/Component.h>
#include "GameObject.h"
class HealthComponent final : public dae::Component
{
public:
	HealthComponent(dae::GameObject* owner, int health) : Component(owner), m_Health(health) 
	{
	
	}
	
	bool TakeDamage(int damage)
	{
		m_Health -= damage;
		if (m_Health <= 0)
		{
			GetOwner().Destroy();
			return true;
		}
		return false;
	}

	int GetHealth() const { return m_Health; }
private:
	int m_Health{};
};
