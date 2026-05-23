#pragma once
#include <Components/Component.h>
#include <GameObject.h>
#include "TankComponent.h"
#include "ObjectTypeComponent.h"
namespace dae
{
	class ShooterComponent final : public Component
	{
	public:
		explicit ShooterComponent(GameObject* owner, ObjectType bulletType)
			:Component(owner), m_TankComponent(owner->GetComponent<TankComponent>()), m_BulletType(bulletType)
		{
		}
		void Update(float dt) override;
		
		void Shoot();
	private:
		TankComponent* m_TankComponent{ nullptr };
		ObjectType m_BulletType{};
	};
}