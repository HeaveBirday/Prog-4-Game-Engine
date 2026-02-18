#pragma once

namespace dae
{
    class GameObject; 

    class Component
    {
    public:
        virtual ~Component() = default;

        virtual void FixedUpdate(float) {}
        virtual void Update(float) {}
        virtual void Render() const {}

        GameObject& GetOwner() const { return *m_owner; }

    private:
        friend class GameObject;

        void SetOwner(GameObject* owner) { m_owner = owner; }

        GameObject* m_owner{ nullptr }; 
    };
}