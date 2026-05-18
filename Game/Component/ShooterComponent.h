#pragma once
#include <Components/Component.h>
#include <GameObject.h>
#include "TankComponent.h"

namespace dae
{
	class ShooterComponent final : public Component
	{
	public:
		explicit ShooterComponent(GameObject* owner)
			:Component(owner), m_TankComponent(owner->GetComponent<TankComponent>())
		{
		}
		void Update(float dt) override;
		
		void Shoot();
	private:
		TankComponent* m_TankComponent{ nullptr };
	};
}