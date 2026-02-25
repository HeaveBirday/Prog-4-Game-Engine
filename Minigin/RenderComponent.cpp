#include "RenderComponent.h"
#include "Renderer.h"
#include "GameObject.h"
#include "TransformComponent.h"


void dae::RenderComponent::Render() const
{
    if (!m_texture) return;

    auto* transform = GetOwner().GetTransform();
    if (!transform) return;


    Renderer::GetInstance().RenderTexture(*m_texture, transform->GetX(), transform->GetY());
}