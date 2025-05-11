#pragma once
#include "Component.h"
#include "Observers.h"



class PlayerHealthComponent;
namespace Engine
{
	class GameObject;
	class TextRenderer;
}

class HealtBarComponent final : public Engine::Component, public Engine::IObserver
{
public:
	HealtBarComponent(Engine::GameObject* pOwner, PlayerHealthComponent* pHealth);
	virtual ~HealtBarComponent();

	void Start() override;

	// Inherited via IObserver
	void OnNotify(Engine::Component* sender) override;
private:
	Engine::TextRenderer* m_pTextComponent;
	PlayerHealthComponent* m_pHealthComponent;

	
};

