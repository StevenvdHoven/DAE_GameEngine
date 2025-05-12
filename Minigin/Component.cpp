#include "Component.h"
#include "GameObject.h"
#include "SceneManager.h"
#include <iostream>

using namespace Engine;

void Engine::Component::Destroy(GameObject* pGameObject)
{
	SceneManager::GetInstance().GetActiveScene()->Remove(pGameObject);

	auto childeren = pGameObject->GetTransform()->GetChildren();
	for (auto& child : childeren)
	{
		SceneManager::GetInstance().GetActiveScene()->Remove(child);
	}
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

void Engine::Component::OnCollisionEnter(GameObject* other)
{
	std::cout << "Collision Enter " << other << std::endl;
}

void Engine::Component::OnCollisionStay(GameObject* other)
{
	std::cout << "Collision Stay " << other << std::endl;
}

void Engine::Component::OnCollisionExit(GameObject* other)
{
	std::cout << "Collision Exit " << other << std::endl;

}

void Engine::Component::OnTriggerEnter(GameObject* other)
{
	std::cout << "Trigger Enter" << other << std::endl;
}

void Engine::Component::OnTriggerStay(GameObject* other)
{
	std::cout << "Trigger Stay " << other << std::endl;
}

void Engine::Component::OnTriggerExit(GameObject* other)
{
	std::cout << "Trigger Exit " << other << std::endl;
}
