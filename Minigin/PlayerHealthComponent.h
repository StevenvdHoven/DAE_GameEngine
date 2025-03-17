#pragma once
#include "Component.h"
#include <memory>
#include "Observers.h"

class PlayerHealthComponent final : public Component
{
public:
	PlayerHealthComponent(GameObject* pOwner, int health);
	virtual ~PlayerHealthComponent();
	int GetHealth() const;
	void TakeDamage(int damage);
	void Heal(int heal);

	Engine::Subject* GetOnTakeDamage() const { return m_OnTakeDamage.get(); }

private:
	int m_Health;
	std::unique_ptr<Engine::Subject> m_OnTakeDamage;
};

