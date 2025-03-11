#include "PlayerHealthComponent.h"
#include "Observers.h"

PlayerHealthComponent::PlayerHealthComponent(GameObject* pOwner, int health):
	Component{ pOwner },
	m_Health{ health },
	m_OnTakeDamage{ std::make_unique<Subject>() }
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
	m_OnTakeDamage->Notify();

	if (m_Health <= 0)
	{
		Destroy(GetGameObject());
	}
}

void PlayerHealthComponent::Heal(int heal)
{
	m_Health += heal;
	m_OnTakeDamage->Notify();
}

