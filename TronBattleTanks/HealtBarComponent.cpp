#include "HealtBarComponent.h"
#include "GameObject.h"
#include "PlayerHealthComponent.h"
#include "TextRenderer.h"


HealtBarComponent::HealtBarComponent(GameObject* pOwner, PlayerHealthComponent* pHealth) :
	Component(pOwner),
	m_pTextComponent{ nullptr },
	m_pHealthComponent{ pHealth }
	
{
	pHealth->GetOnTakeDamage()->AddObserver(this);
}

HealtBarComponent::~HealtBarComponent()
{
	m_pTextComponent = nullptr;
	m_pHealthComponent = nullptr;
}

void HealtBarComponent::Start()
{
	m_pTextComponent = GetGameObject()->GetComponent<TextRenderer>();
}

void HealtBarComponent::OnNotify()
{
	if (m_pTextComponent)
	{
		m_pTextComponent->SetText("Health: " + std::to_string(m_pHealthComponent->GetHealth()));
	}
}

