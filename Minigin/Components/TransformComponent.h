#pragma once
#include "Component.h"
#include <glm/glm.hpp>
namespace dae
{
    class TransformComponent final : public Component
    {
    public:
		explicit TransformComponent(GameObject* owner) : Component(owner) {}
        //Local position (relative to parent)
        void SetLocalPosition(float x, float y)
        {
            m_LocalPos.x = x;
            m_LocalPos.y = y;
            MarkDirty();
        }

        void AddLocalOffset(float dx, float dy)
        {
            m_LocalPos.x += dx;
            m_LocalPos.y += dy;
            MarkDirty();
        }

        float GetLocalX() const { return m_LocalPos.x; }
        float GetLocalY() const { return m_LocalPos.y; }

        //World position
        void SetPosition(float x, float y) { SetWorldPosition(x, y); }
        void SetPosition(glm::vec2 vectorPos) { SetWorldPosition(vectorPos.x, vectorPos.y); }

        void SetWorldPosition(glm::vec2 pos);
        void SetWorldPosition(float x, float y);


        float GetX() const { return GetWorldX(); }
        float GetY() const { return GetWorldY(); }

        float GetWorldX() const
        {
            UpdateWorldIfDirty();
            return m_WorldPos.x;
        }

        glm::vec2 GetWorldPosition() const
        {
            UpdateWorldIfDirty();
            return m_WorldPos;
		}
        float GetWorldY() const
        {
            UpdateWorldIfDirty();
            return m_WorldPos.y;
        }
        void RequiresUpdate() { MarkDirty(); }

        void GetWorldPosition(float& x, float& y) const
        {
            x = GetWorldX();
            y = GetWorldY();
        }
    private:
        void MarkDirty() const;

        void UpdateWorldIfDirty() const;

		glm::vec2 m_LocalPos{ 0.f, 0.f };
		//TODO change pos with glm vectors instead of separate floats
		mutable glm::vec2 m_WorldPos{ 0.f, 0.f };
        mutable bool  m_dirty{ true };
    };
}