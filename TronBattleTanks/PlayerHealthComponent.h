#pragma once
#include "Component.h"
#include <memory>
#include "Observers.h"
#include "SoundSystem.h"



class PlayerHealthComponent final : public Engine::Component
{
public:
	PlayerHealthComponent(Engine::GameObject* pOwner);
	void TakeDamage(int damage);

	Engine::Subject* GetOnTakeDamage() const { return m_OnTakeDamage.get(); }

	std::string GetTypeName() const override;

private:
	std::unique_ptr<Engine::Subject> m_OnTakeDamage;
	Engine::SoundClip m_ExplosionSound;
	
};

