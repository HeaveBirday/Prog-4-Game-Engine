#pragma once
#include "Component.h"
#include "../IEventListener.h"

namespace dae
{
    class LivesDisplayComponent final
        : public Component
        , public IEventListener
    {
    public:
        LivesDisplayComponent(GameObject* owner, int playerId);
        ~LivesDisplayComponent() override;

        void OnEvent(const Event& event) override;

    private:
        void UpdateText();

        int m_PlayerId{};
        int m_Lives{ 3 };
    };
}