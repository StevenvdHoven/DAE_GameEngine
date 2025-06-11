#include "SimpleTriggerComponent.h"
#include "PlayerHealthComponent.h"
#include "GameObject.h"

SimpleTriggerComponent::SimpleTriggerComponent(Engine::GameObject* pOwner):
	Engine::Component{pOwner},
	m_OnTrigger{std::make_unique<Engine::Subject>()},
	m_LastHit{nullptr}
{
}

void SimpleTriggerComponent::OnTriggerEnter(Engine::GameObject* other)
{
	auto player{ other->GetComponent<PlayerHealthComponent>() };
	if (player)
	{
		m_LastHit = other;
		m_OnTrigger->Notify(this);
	}
}

std::string SimpleTriggerComponent::GetTypeName() const
{
	return "SimpleTriggerComponent";
}
