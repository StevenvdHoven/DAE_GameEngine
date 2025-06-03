#pragma once
#include "Component.h"
#include "IDamageable.h"
#include "Observers.h"
#include <memory>

class EnemyHealthComponent final : public Engine::Component, public  IDamageable
{
public:
	EnemyHealthComponent(Engine::GameObject* pOwner, int health = 3);


	// Inherited via IDamageable
	void TakeDamage(int damage) override;
	Engine::Subject& OnTakeDamage() const;
private:
	mutable int m_Health;
	std::unique_ptr<Engine::Subject> OnTakeDamageEvent;
};

