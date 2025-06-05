#pragma once
#include "Component.h"
#include <memory>
#include "Observers.h"



class PlayerHealthComponent final : public Engine::Component
{
public:
	PlayerHealthComponent(Engine::GameObject* pOwner);
	void TakeDamage(int damage);

	Engine::Subject* GetOnTakeDamage() const { return m_OnTakeDamage.get(); }

private:
	std::unique_ptr<Engine::Subject> m_OnTakeDamage;
};

