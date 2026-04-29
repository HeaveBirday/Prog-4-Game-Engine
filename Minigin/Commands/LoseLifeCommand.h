#pragma once
#include "Command.h"

namespace dae
{
    class GameObject;
}

class LoseLifeCommand final : public Command
{
public:
    explicit LoseLifeCommand(dae::GameObject* playerObject);
    void Execute(float deltaTime) override;

private:
    dae::GameObject* m_PlayerObject{};
};