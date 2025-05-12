#include "PlayerHealthComponent.h"
#include "Observers.h"

using namespace Engine;

PlayerHealthComponent::PlayerHealthComponent(GameObject* pOwner, int health):
	Component{ pOwner },
	m_Health{ health },
	m_OnTakeDamage{ std::make_unique<Engine::Subject>() }
{
}

PlayerHealthComponent::~PlayerHealthComponent()
{
}

int PlayerHealthComponent::GetHealth() const
{
	return m_Health;
}

void PlayerHealthComponent::TakeDamage(int damage)
{
	m_Health -= damage;
	m_OnTakeDamage->Notify(this);
}

void PlayerHealthComponent::Heal(int heal)
{
	m_Health += heal;
	m_OnTakeDamage->Notify();
}

