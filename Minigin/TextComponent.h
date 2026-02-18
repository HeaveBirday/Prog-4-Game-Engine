#pragma once
#include "Component.h"
#include <memory>
#include <string>
#include <SDL3/SDL.h>

namespace dae
{
    class Font;
    class Texture2D;

    class TextComponent final : public Component
    {
    public:
        TextComponent(std::shared_ptr<Font> font, SDL_Color color = { 255,255,255,255 });

        void Update(float) override;
        void Render() const override;

        void SetText(const std::string& text);
        void SetColor(const SDL_Color& color);

    private:
        bool m_needsUpdate{ true };
        std::string m_text{ "FPS: 0" };
        SDL_Color m_color{ 255,255,255,255 };
        std::shared_ptr<Font> m_font{};
        std::shared_ptr<Texture2D> m_textTexture{};
    };
}