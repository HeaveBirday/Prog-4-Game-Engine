#pragma once
#include "Component.h"
#include <memory>


namespace dae
{
    class Texture2D;

    class RenderComponent final : public Component
    {
    public:
        explicit RenderComponent(GameObject* owner, std::shared_ptr<Texture2D> texture)
            : Component(owner),m_texture(std::move(texture))
        {
        }

        void Render() const override;

    private:
        std::shared_ptr<Texture2D> m_texture;
    };
}