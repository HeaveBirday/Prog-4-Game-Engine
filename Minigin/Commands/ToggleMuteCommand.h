#pragma once
#include <Commands/Command.h>
#include <ServiceLocator.h>

class ToggleMuteCommand final : public Command
{
public:
	void Execute(float) override
	{
		dae::ServiceLocator::GetSoundSystem().ToggleMuted();
	}
};