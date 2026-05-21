#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <vector>
#include <SDL3/SDL_log.h>
namespace dae
{
	using CollisionLayer = unsigned int;
	class CollisionComponent final : public Component
	{
	public:
		explicit CollisionComponent(GameObject* owner, glm::vec2 dimensions, CollisionLayer layer)
			:Component(owner), 
			m_CollisionRect(0.f,
							0.f,
							dimensions.x,
							dimensions.y),
			m_CollisionLayer(layer)

		{
			s_AllColliders.push_back(this);
			SDL_Log("Collider created, layer: %u", m_CollisionLayer);
		}
		CollisionComponent(GameObject* owner, glm::vec4 rect, CollisionLayer layer)
			:Component(owner), m_CollisionRect(rect), m_CollisionLayer(layer)
		{
			s_AllColliders.push_back(this);
			SDL_Log("Collider created, layer: %u", m_CollisionLayer);
			
		}
		~ CollisionComponent()
		{
			s_AllColliders.erase(std::remove(s_AllColliders.begin(), s_AllColliders.end(), this), s_AllColliders.end());
		}

		void Update(float dt) override;
		bool CheckCollision(const CollisionComponent& otherComponent) const;
		glm::vec4 GetBoundingBox() const;
		CollisionLayer GetCollisionLayer() const { return m_CollisionLayer; }

	private:
		glm::vec4 m_CollisionRect;
		static inline std::vector<CollisionComponent*> s_AllColliders{};
		CollisionLayer m_CollisionLayer{};
	};

}