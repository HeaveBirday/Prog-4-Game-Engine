#include "EventManager.h"
#include <algorithm>

void dae::EventManager::AddListener(IEventListener* listener)
{
    if (!listener) return;

    auto it = std::find(m_Listeners.begin(), m_Listeners.end(), listener);
    if (it == m_Listeners.end())
    {
        m_Listeners.push_back(listener);
    }
}

void dae::EventManager::RemoveListener(IEventListener* listener)
{
    auto it = std::remove(m_Listeners.begin(), m_Listeners.end(), listener);
    m_Listeners.erase(it, m_Listeners.end());
}

void dae::EventManager::QueueEvent(const Event& event)
{
    m_EventQueue.push(event);
}

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