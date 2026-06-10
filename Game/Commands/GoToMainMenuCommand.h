#pragma once
#include <Commands/Command.h>

class GameStateManager;

class GoToMainMenuCommand final : public Command
{
public:
    explicit GoToMainMenuCommand();

    void Execute(float deltaTime) override;

private:

};