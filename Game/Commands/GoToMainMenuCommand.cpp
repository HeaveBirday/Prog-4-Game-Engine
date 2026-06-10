#include "GoToMainMenuCommand.h"
#include "../States/GameStateManager.h"
#include "../States/MainMenuState.h"
extern GameStateManager g_GameStateManager;
GoToMainMenuCommand::GoToMainMenuCommand()
{
}

void GoToMainMenuCommand::Execute(float)
{
	g_GameStateManager.SetState(std::make_unique<MainMenuState>());
}