#include "PlayerDamageCommand.h"
#include "PlayerHealthComponent.h"

using namespace Engine;

PlayerDamageCommand::PlayerDamageCommand(GameObject* pActor) :
	GameActorCommand{ pActor }
{
	m_pHealthComponent = pActor->GetComponent<PlayerHealthComponent>();
}

void PlayerDamageCommand::Execute()
{
	if (m_pHealthComponent)
	{
		m_pHealthComponent->TakeDamage(1);
	}
}
