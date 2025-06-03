#include "EnemyHealthComponent.h"

EnemyHealthComponent::EnemyHealthComponent(Engine::GameObject* pOwner, int health):
	Engine::Component(pOwner),
	m_Health{health}
{
}

void EnemyHealthComponent::TakeDamage(int damage)
{
	m_Health -= damage;
	OnTakeDamageEvent->Notify();
	if (m_Health <= 0)
	{
		Destroy(GetGameObject());
	}
}

Engine::Subject& EnemyHealthComponent::OnTakeDamage() const
{
	return *OnTakeDamageEvent;
}
