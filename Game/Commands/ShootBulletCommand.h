#pragma once
#include <Commands/Command.h>
namespace dae
{
	class GameObject;

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
