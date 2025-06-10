#include "EnemyHealthComponent.h"
#include "ServiceLocator.h"

EnemyHealthComponent::EnemyHealthComponent(Engine::GameObject* pOwner, int health):
	Engine::Component(pOwner),
	m_Health{health},
	m_ExplosionClip{Engine::ServiceLocator::GetSoundSystem().LoadSound("explosion.wav")},
	OnTakeDamageEvent{std::make_unique<Engine::Subject>()}
{
}

void EnemyHealthComponent::TakeDamage(int damage)
{
	m_Health -= damage;
	OnTakeDamageEvent->Notify(this);
	if (m_Health <= 0)
	{
		Engine::ServiceLocator::GetSoundSystem().PlaySound(m_ExplosionClip);
		Destroy(GetGameObject());
	}
}

Engine::Subject& EnemyHealthComponent::OnTakeDamage() const
{
	return *OnTakeDamageEvent;
}

std::string EnemyHealthComponent::GetTypeName() const
{
	return "EnemyHealthComponent";
}
