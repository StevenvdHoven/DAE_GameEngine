#include "Component.h"
#include "GameObject.h"
#include "SceneManager.h"
#include <iostream>

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

void Engine::Component::OnCollisionEnter(Component* other)
{
	std::cout << "Collision Enter" << other << std::endl;
}

void Engine::Component::OnCollisionStay(Component* other)
{
	std::cout << "Collision Stay" << other << std::endl;
}

void Engine::Component::OnCollisionExit(Component* other)
{
	std::cout << "Collision Exit" << other << std::endl;

}

void Engine::Component::OnTriggerEnter(Component* other)
{
	std::cout << "Trigger Enter" << other << std::endl;
}

void Engine::Component::OnTriggerStay(Component* other)
{
	std::cout << "Trigger Stay" << other << std::endl;
}

void Engine::Component::OnTriggerExit(Component* other)
{
	std::cout << "Trigger Exit" << other << std::endl;
}
