#pragma once
#include "Singleton.h"
#include "Event.h"
#include "IEventListener.h"
#include <queue>
#include <vector>

namespace dae
{
	// EventManager is a singleton class responsible for managing events and their listeners in the game
    // Central event queue used to decouple systems.
    // Objects can queue events, and listeners receive them later through ProcessEvents().
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