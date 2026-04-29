#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <vector>
namespace dae
{
	class CollisionComponent final : public Component
	{
	public:
		explicit CollisionComponent(GameObject* owner, glm::vec2 dimensions)
			:Component(owner), m_CollisionRect(-(dimensions.x / 2), -(dimensions.y / 2), dimensions.x / 2, dimensions.y / 2)
		{
			s_AllColliders.push_back(this);
		}
		CollisionComponent(GameObject* owner, glm::vec4 rect)
			:Component(owner), m_CollisionRect(rect)
		{
			s_AllColliders.push_back(this);
		}
		~ CollisionComponent()
		{
			s_AllColliders.erase(std::remove(s_AllColliders.begin(), s_AllColliders.end(), this), s_AllColliders.end());
		}
		void Update(float dt) override;
		bool CheckCollision(const CollisionComponent& otherComponent) const;
		glm::vec4 GetBoundingBox() const;

	private:
		glm::vec4 m_CollisionRect;
		static inline std::vector<CollisionComponent*> s_AllColliders{};
	};

}