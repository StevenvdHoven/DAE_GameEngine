#include "AutoDestroyComponent.h"
#include "EngineTime.h"
#include "ServiceLocator.h"

AutoDestroyComponent::AutoDestroyComponent(Engine::GameObject* pOwner) :
	AutoDestroyComponent{ pOwner,1.f }
{
}

AutoDestroyComponent::AutoDestroyComponent(Engine::GameObject* pOwner, float lifeTime) :
	Engine::Component{ pOwner },
	m_CountingDown{ false },
	m_LifeTime{ lifeTime }
{
}

void AutoDestroyComponent::Start()
{
	if (!Engine::ServiceLocator::GetLevelEditor().IsActive())
		m_CountingDown = true;
}

void AutoDestroyComponent::Update()
{
	if (m_CountingDown)
	{
		m_LifeTime -= Engine::Time::GetInstance().GetDeltaTime();
		if (m_LifeTime <= 0.f)
		{
			Destroy(GetGameObject());
			return;
		}
	}
}

std::string AutoDestroyComponent::GetTypeName() const
{
	return "AutoDestroyComponent";
}
