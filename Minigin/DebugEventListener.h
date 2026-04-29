#pragma once
#include "IEventListener.h"
#include <SDL3/SDL.h>


namespace dae
{
    class DebugEventListener final : public IEventListener
    {
    public:
        void OnEvent(const Event& event) override
        {
            switch (event.id)
            {
            case make_sdbm_hash("PlayerDied"):
                
                SDL_Log("Event received: PlayerDied | playerId = %d", event.playerId);

                break;
            case make_sdbm_hash("LivesChanged"):
                SDL_Log("Event received: LivesChanged | playerId = %d | lives = %d", event.playerId, event.value);

                break;
            case make_sdbm_hash("ScoreChanged"):
                SDL_Log("Event received: ScoreChanged | playerId = %d | score = %d", event.playerId, event.value);

                break;
            }
        }
    };
}