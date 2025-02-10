#include "Component.h"
#include "GameObject.h"
#include "SceneManager.h"

void Component::Destroy(GameObject* pGameObject)
{
	SceneManager::GetInstance().GetActiveScene()->Remove(pGameObject);
}

void Component::Destroy(Component* pComponent)
{
	pComponent->GetGameObject()->RemoveComponent(pComponent);
}
