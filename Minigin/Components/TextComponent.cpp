#include "TextComponent.h"
#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "TransformComponent.h"

//THIS TextComponent IS PRETTY MUCH TextObject BUT MADE INTO A COMPONENT
dae::TextComponent::TextComponent(GameObject* owner,Font* font, const SDL_Color& color)
    : Component(owner)
    , m_font(font)
    , m_color(color)
{

}

void dae::TextComponent::Update(float)
{
    if (!m_needsUpdate) return;

    const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_text.length(), m_color);
    if (!surf) throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());

    auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
    if (!texture) throw std::runtime_error(std::string("Create text texture failed: ") + SDL_GetError());

    SDL_DestroySurface(surf);
    m_textTexture = std::make_unique<Texture2D>(texture);
    m_needsUpdate = false;
}

void dae::TextComponent::Render() const
{
    if (!m_textTexture) return;

    auto* tr = GetOwner().GetComponent<TransformComponent>();
    if (!tr) return;

    Renderer::GetInstance().RenderTexture(*m_textTexture, tr->GetX(), tr->GetY());
}

void dae::TextComponent::SetText(const std::string& text)
{
    m_text = text;
    m_needsUpdate = true;
}

void dae::TextComponent::SetColor(const SDL_Color& color)
{
    m_color = color;
    m_needsUpdate = true;
}