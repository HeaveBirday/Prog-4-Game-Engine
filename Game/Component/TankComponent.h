#pragma once
#include "Components/Component.h"
#include <glm/glm.hpp>
#include <SDL3/SDL.h>
namespace dae
{
	enum class Direction
	{
		Up,
		Down,
		Left,
		Right
	};
	class TankComponent final : public Component
	{
	public:
		explicit TankComponent(GameObject* owner, glm::vec2 size)
			: Component(owner),
			m_Size(size)
		{

		}
		void SetDirection(Direction direction)
		{
			m_Direction = direction;
		}
		Direction GetDirection() const
		{
			return m_Direction;
		}
		glm::vec2 GetSize() const
		{
			return m_Size;
		}
	private:
		Direction m_Direction{ Direction::Up };
		glm::vec2 m_Size{};

	};
	
}