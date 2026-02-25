#pragma once
#include "Component.h"

namespace dae
{
    class TransformComponent final : public Component
    {
    public:
        //Local position (relative to parent)
        void SetLocalPosition(float x, float y)
        {
            m_localX = x;
            m_localY = y;
            MarkDirty();
        }

        void AddLocalOffset(float dx, float dy)
        {
            m_localX += dx;
            m_localY += dy;
            MarkDirty();
        }

        float GetLocalX() const { return m_localX; }
        float GetLocalY() const { return m_localY; }

        //World position
        void SetPosition(float x, float y) { SetWorldPosition(x, y); }

        void SetWorldPosition(float x, float y);

        float GetX() const { return GetWorldX(); }
        float GetY() const { return GetWorldY(); }

        float GetWorldX() const
        {
            UpdateWorldIfDirty();
            return m_worldX;
        }

        float GetWorldY() const
        {
            UpdateWorldIfDirty();
            return m_worldY;
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

        float m_localX{ 0.f };
        float m_localY{ 0.f };

        mutable float m_worldX{ 0.f };
        mutable float m_worldY{ 0.f };
        mutable bool  m_dirty{ true };
    };
}