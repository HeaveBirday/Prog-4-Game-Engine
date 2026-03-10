#pragma once
#include "Command.h"
#include <SDL3/SDL.h>

class PrintCommand final : public Command
{
public:
	void Execute() override
	{
		SDL_Log("Command executed!");
	}
};