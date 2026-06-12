#pragma once
#include <Commands/Command.h>
namespace dae
{
	class GameObject;
	// ShootBulletCommand is a command that, when executed, will make the player shoot a bullet.
	// It holds a reference to the player's GameObject to access necessary components for shooting.
	class ShootBulletCommand final : public Command
	{
	public:
		explicit ShootBulletCommand(dae::GameObject* playerObject)
		{
			m_PlayerObject = playerObject;
		}
		void Execute(float deltaTime) override;
	private:
		dae::GameObject* m_PlayerObject{};


	};
}
