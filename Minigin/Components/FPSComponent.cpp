#include "FPSComponent.h"
#include <string>
#include "GameObject.h"
#include "TextComponent.h"

void dae::FPSComponent::Update(float dt)
{
   m_accum += dt;
    ++m_frames;

    if (m_accum >= m_refreshInterval)
    {
        const float fps = static_cast<float>(m_frames) / m_accum;
        //const int rounded = static_cast<int>(fps + 0.5f);

        if (auto* text = GetOwner().GetComponent<TextComponent>())
        {
			text->SetText(std::to_string(fps) + " FPS");
        }
        //text->SetText(std::to_string(rounded) + " FPS");
     
        m_accum = 0.f;
        m_frames = 0;
    }
   //Debug Purposes
   /* if (auto* text = GetOwner().GetComponent<TextComponent>())
    {
        static int i = 0;
        text->SetText("tick " + std::to_string(i++));
    }*/
}