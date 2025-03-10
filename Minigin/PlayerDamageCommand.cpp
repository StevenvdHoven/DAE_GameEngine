#include "PlayerDamageCommand.h"
#include "PlayerHealthComponent.h"

Engine::PlayerDamageCommand::PlayerDamageCommand(GameObject* pActor):
	GameActorCommand{ pActor }
{
	m_pHealthComponent = pActor->GetComponent<PlayerHealthComponent>();
}

void Engine::PlayerDamageCommand::Execute()
{
	if (m_pHealthComponent)
	{
		m_pHealthComponent->TakeDamage(1);
	}
}
