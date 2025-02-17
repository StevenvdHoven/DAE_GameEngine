#include "Component.h"
#include "GameObject.h"
#include "SceneManager.h"

void Component::Destroy(GameObject* pGameObject)
{
	SceneManager::GetInstance().GetActiveScene()->Remove(pGameObject);
}

void Component::Destroy(Component* pComponent)
{
	if (pComponent == this)
	{
		m_Destroyed = true;
		return;
	}

	pComponent->GetGameObject()->RemoveComponent(pComponent);
}

Component::Component(GameObject* pOwner):
	m_pGameObject{ pOwner },
	m_pTransform{ pOwner->GetTransform() },
	m_Destroyed{ false }
{
}
