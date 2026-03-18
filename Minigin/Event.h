#pragma once

namespace dae
{
    enum class EventType
    {
        PlayerDied,
        LivesChanged,
        ScoreChanged
    };

    struct Event
    {
        EventType type{};
        int playerId{ -1 };
        int value{ 0 };
    };
}