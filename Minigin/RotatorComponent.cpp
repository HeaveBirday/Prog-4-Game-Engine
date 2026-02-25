#include "RotatorComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include <cmath>

void dae::RotatorComponent::Update(float dt)
{
	{
		m_angle += m_speed * dt;

		// Local position relative to parent (or origin if no parent)
		const float x = std::cos(m_angle) * m_radius;
		const float y = std::sin(m_angle) * m_radius;

		// IMPORTANT: local so parenting just works
		if (auto* tr = GetOwner().GetTransform())
			tr->SetLocalPosition(x, y);
	}
}
