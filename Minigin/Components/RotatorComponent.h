#pragma once
#include "Component.h"

namespace dae
{
	class RotatorComponent final : public Component
	{
	public:
		// radius in pixels, speed in radians/sec
		RotatorComponent(GameObject* owner, float radius, float speed, float startAngle = 0.f)
			: Component(owner), m_radius(radius), m_speed(speed), m_angle(startAngle)
		{
		}

		void Update(float dt) override;
		

	private:
		float m_radius{};
		float m_speed{};
		float m_angle{};
	};
}