#pragma once
#include "Singleton.h"
#include "Event.h"
#include "IEventListener.h"
#include <queue>
#include <vector>

namespace dae
{
    class EventManager final : public Singleton<EventManager>
    {
    public:
        void AddListener(IEventListener* listener);
        void RemoveListener(IEventListener* listener);

        void QueueEvent(const Event& event);
        void ProcessEvents();

    private:
        std::queue<Event> m_EventQueue{};
        std::vector<IEventListener*> m_Listeners{};
    };
}