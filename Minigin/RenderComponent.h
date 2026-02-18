#pragma once
#include "Component.h"
#include <memory>


namespace dae
{
    class Texture2D;

    class RenderComponent final : public Component
    {
    public:
        explicit RenderComponent(std::shared_ptr<Texture2D> texture)
            : m_texture(std::move(texture))
        {
        }

        void Render() const override;

    private:
        std::shared_ptr<Texture2D> m_texture;
    };
}