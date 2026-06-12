#pragma once
#include <Components/Component.h>
#include "GameObject.h"
#include <ServiceLocator.h>
#include "../SoundIds.h"

// HealthComponent manages the health of a GameObject, allowing it to take damage and be destroyed when health reaches zero. It also plays a sound effect when taking damage.
class HealthComponent final : public dae::Component
{
public:
	HealthComponent(dae::GameObject* owner, int health) : Component(owner), m_Health(health) 
	{
	
	}
	
	bool TakeDamage(int damage)
	{
		dae::ServiceLocator::GetSoundSystem().Play(dae::SoundIds::EnemyHit, 1.f);
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
