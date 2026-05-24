#pragma once
#include <glm/glm.hpp>
#include "Component.h"
#include <memory>


namespace dae
{
    class Texture2D;

    class RenderComponent final : public Component
    {
    public:
        explicit RenderComponent(GameObject* owner, std::shared_ptr<Texture2D> texture)
            : Component(owner),m_texture(std::move(texture)), m_Scale(1.f,1.f)
        {
        }

        void Render() const override;

        void SetScale(glm::vec2 scale)
        {
            m_Scale = scale;
        }

    private:
        std::shared_ptr<Texture2D> m_texture;
        glm::vec2 m_Scale{};
    };
}