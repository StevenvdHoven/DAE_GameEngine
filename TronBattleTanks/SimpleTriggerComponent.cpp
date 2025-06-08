#include "SimpleTriggerComponent.h"

SimpleTriggerComponent::SimpleTriggerComponent(Engine::GameObject* pOwner):
	Engine::Component{pOwner},
	m_OnTrigger{std::make_unique<Engine::Subject>()},
	m_LastHit{nullptr}
{
}

void SimpleTriggerComponent::OnTriggerEnter(Engine::GameObject* other)
{
	m_LastHit = other;
	m_OnTrigger->Notify(this);
}

std::string SimpleTriggerComponent::GetTypeName() const
{
	return "SimpleTriggerComponent";
}
