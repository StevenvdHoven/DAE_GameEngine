#include "PlayerHealthComponent.h"
#include "Observers.h"

using namespace Engine;

PlayerHealthComponent::PlayerHealthComponent(GameObject* pOwner):
	Component{ pOwner },
	m_OnTakeDamage{ std::make_unique<Engine::Subject>() }
{
}

void PlayerHealthComponent::TakeDamage(int)
{
	m_OnTakeDamage->Notify(this);
}

