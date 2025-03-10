#pragma once
#include "Component.h"
#include "Observers.h"

class PlayerHealthComponent;
class TextRenderer;
class HealtBarComponent final : public Component, public IObserver
{
public:
	HealtBarComponent(GameObject* pOwner, PlayerHealthComponent* pHealth);
	virtual ~HealtBarComponent();

	void Start() override;

	// Inherited via IObserver
	void OnNotify() override;
private:
	TextRenderer* m_pTextComponent;
	PlayerHealthComponent* m_pHealthComponent;

	
};

