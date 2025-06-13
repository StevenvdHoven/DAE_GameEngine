#include "PlayerHealthComponent.h"
#include "Observers.h"
#include "ServiceLocator.h"

using namespace Engine;

PlayerHealthComponent::PlayerHealthComponent(GameObject* pOwner):
	Component{ pOwner },
	m_OnTakeDamage{ std::make_unique<Engine::Subject>() },
	m_ExplosionSound{ ServiceLocator::GetSoundSystem().LoadSound("explosion.wav") }
{
}

void PlayerHealthComponent::TakeDamage(int)
{
	ServiceLocator::GetSoundSystem().PlaySound(m_ExplosionSound);
	m_OnTakeDamage->Notify(this);
}

std::string PlayerHealthComponent::GetTypeName() const
{
	return "PlayerHealthComponent";
}

