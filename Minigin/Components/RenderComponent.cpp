#include "RenderComponent.h"
#include "Renderer.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Texture2D.h"

void dae::RenderComponent::Render() const
{
    if (!m_texture) return;

    auto* transform = GetOwner().GetTransform();
    if (!transform) return;
    
    float width{};
	float height{};
	SDL_GetTextureSize(m_texture->GetSDLTexture(), &width, &height);


    Renderer::GetInstance().RenderTexture(*m_texture,
        transform->GetX(), 
        transform->GetY(),
        width,
        height,
        transform->GetRotation());
}