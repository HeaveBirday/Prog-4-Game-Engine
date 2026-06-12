#pragma once
#include <Components/Component.h>
#include <GameObject.h>
#include "TankComponent.h"
#include "ObjectTypeComponent.h"
namespace dae
{
	// ShooterComponent is responsible for handling the shooting mechanics of a tank,
	// including bullet creation and direction based on the tank's orientation.
	// It interacts closely with the TankComponent to determine when shooting is possible and to reset cooldowns.
	class ShooterComponent final : public Component
	{
	public:
		explicit ShooterComponent(GameObject* owner, ObjectType bulletType, bool useTurretDirection = false)
			:Component(owner), m_TankComponent(owner->GetComponent<TankComponent>()), m_BulletType(bulletType), m_UseTurretDirection(useTurretDirection)
		{
		}
		void Update(float dt) override;
		
		void Shoot();
	private:
		TankComponent* m_TankComponent{ nullptr };
		ObjectType m_BulletType{};
		bool m_UseTurretDirection{};
	};
}