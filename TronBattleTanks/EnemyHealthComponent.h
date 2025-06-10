#pragma once
#include "Component.h"
#include "IDamageable.h"
#include "Observers.h"
#include "SoundSystem.h"
#include <memory>

class EnemyHealthComponent final : public Engine::Component, public  IDamageable
{
public:
	EnemyHealthComponent(Engine::GameObject* pOwner, int health = 3);

	bool IsDead() const { return m_Health <= 0; }

	// Inherited via IDamageable
	void TakeDamage(int damage) override;
	Engine::Subject& OnTakeDamage() const;


	std::string GetTypeName() const override;
private:
	mutable int m_Health;
	Engine::SoundClip m_ExplosionClip;
	std::unique_ptr<Engine::Subject> OnTakeDamageEvent;
};

