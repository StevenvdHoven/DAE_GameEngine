#include "Component.h"
#include "GameObject.h"
#include "SceneManager.h"
#include <iostream>
#include "json.hpp"

using namespace Engine;

void Engine::Component::Destroy(GameObject* pGameObject)
{
	SceneManager::GetInstance().GetActiveScene()->Remove(pGameObject);

	
}

void Engine::Component::Destroy(Component* pComponent)
{
	if (pComponent == this)
	{
		m_Destroyed = true;
		return;
	}

	pComponent->GetGameObject()->RemoveComponent(pComponent);
}

Engine::Component::Component(GameObject* pOwner):
	IsEnabled{ true },
	m_pGameObject{ pOwner },
	m_Destroyed{ false }
{
}

void Engine::Component::OnCollisionEnter(GameObject*)
{
	
}

void Engine::Component::OnCollisionStay(GameObject*)
{
	
}

void Engine::Component::OnCollisionExit(GameObject*)
{
	

}

void Engine::Component::OnTriggerEnter(GameObject*)
{
	
}

void Engine::Component::OnTriggerStay(GameObject*)
{
	
}

void Engine::Component::OnTriggerExit(GameObject*)
{
	
}
