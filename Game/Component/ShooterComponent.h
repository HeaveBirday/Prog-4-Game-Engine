#pragma once
#include <Components/Component.h>
#include <GameObject.h>

namespace dae
{
	class ShooterComponent final : public Component
	{
	public:
		explicit ShooterComponent(GameObject* owner)
			:Component(owner)
		{
		}
		void Update(float dt) override;
		
		void Shoot();
	private:
		
	};
}