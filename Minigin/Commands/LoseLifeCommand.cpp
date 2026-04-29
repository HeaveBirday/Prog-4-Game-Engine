#include "LoseLifeCommand.h"
#include "GameObject.h"
#include "Components/HealthComponent.h"

LoseLifeCommand::LoseLifeCommand(dae::GameObject* playerObject)
    : m_PlayerObject(playerObject)
{
}

void LoseLifeCommand::Execute(float)
{
    if (!m_PlayerObject) return;

    auto* health = m_PlayerObject->GetComponent<dae::HealthComponent>();
    if (!health) return;

    health->LoseLife();
}