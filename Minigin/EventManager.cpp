#include "EventManager.h"
#include <algorithm>

// The EventManager class is responsible for managing event listeners and processing events in the game. 
// It allows listeners to subscribe to events and ensures that they receive notifications when relevant events occur. 
// The implementation uses a queue to store events and a vector to manage listeners, 
// providing efficient event handling while maintaining a clean separation of concerns between event generation and event processing.
void dae::EventManager::AddListener(IEventListener* listener)
{
    if (!listener) return;

    auto it = std::find(m_Listeners.begin(), m_Listeners.end(), listener);
    if (it == m_Listeners.end())
    {
        m_Listeners.push_back(listener);
    }
}
// RemoveListener method removes a listener from the list of event listeners, ensuring that it will no longer receive event notifications.
void dae::EventManager::RemoveListener(IEventListener* listener)
{
	if (m_Listeners.empty() || !listener) return;
    auto it = std::remove(m_Listeners.begin(), m_Listeners.end(), listener);
    m_Listeners.erase(it, m_Listeners.end());
}

// QueueEvent method adds an event to the event queue, allowing it to be processed later.
void dae::EventManager::QueueEvent(const Event& event)
{
    m_EventQueue.push(event);
}
// ProcessEvents method processes all events in the event queue, notifying all registered listeners of each event and ensuring that they can respond accordingly.
void dae::EventManager::ProcessEvents()
{
    while (!m_EventQueue.empty())
    {
        Event event = m_EventQueue.front();
        m_EventQueue.pop();

        for (auto* listener : m_Listeners)
        {
            if (listener)
            {
                listener->OnEvent(event);
            }
        }
    }
}