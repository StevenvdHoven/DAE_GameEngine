#include <string>
#include "GameObject.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Component.h"

using namespace Engine;

Engine::GameObject::GameObject()
{
	m_pTransform = AddComponent<Transform>();
}

Engine::GameObject::~GameObject()
{

}

void Engine::GameObject::RemoveComponent(Component* pComponent)
{
	pComponent->Destroy(pComponent);
}

void Engine::GameObject::Start()
{
	if (!m_IsActive) return;

	for (auto& pComponent : m_Components)
	{
		pComponent->Start();
	}
}

void Engine::GameObject::Update()
{
	if (!m_IsActive) return;

	for (auto& pComponent : m_Components)
	{
		if (pComponent->IsEnabled)
			pComponent->Update();
	}
}

void Engine::GameObject::FixedUpdate()
{
	if (!m_IsActive) return;

	for (auto& pComponent : m_Components)
	{
		if (pComponent->IsEnabled)
			pComponent->FixedUpdate();
	}
}

void Engine::GameObject::LateUpdate()
{
	if (!m_IsActive) return;

	for (auto& pComponent : m_Components)
	{
		if (pComponent->IsEnabled)
			pComponent->LateUpdate();
	}

	m_Components.erase(std::remove_if(m_Components.begin(), m_Components.end(),
		[](const std::unique_ptr<Component>& pComponent)
		{
			return pComponent->IsDestroyed();
		}), m_Components.end());
}

void Engine::GameObject::Render() const
{
	if (!m_IsActive) return;

	for (auto& pComponent : m_Components)
	{
		if (pComponent->IsEnabled)
			pComponent->Render();
	}
}

void Engine::GameObject::OnCollisionEnter(GameObject* other)
{
	if (!m_IsActive) return;

	for (auto& pComponent : m_Components)
	{
		if (pComponent->IsEnabled)
			pComponent->OnCollisionEnter(other);
	}
}

void Engine::GameObject::OnCollisionStay(GameObject* other)
{
	if (!m_IsActive) return;

	for (auto& pComponent : m_Components)
	{
		if (pComponent->IsEnabled)
			pComponent->OnCollisionStay(other);
	}
}

void Engine::GameObject::OnCollisionExit(GameObject* other)
{
	if (!m_IsActive) return;

	for (auto& pComponent : m_Components)
	{
		if (pComponent->IsEnabled)
			pComponent->OnCollisionExit(other);
	}
}

void Engine::GameObject::OnTriggerEnter(GameObject* other)
{
	if (!m_IsActive) return;

	for (auto& pComponent : m_Components)
	{
		if (pComponent->IsEnabled)
			pComponent->OnTriggerEnter(other);
	}
}

void Engine::GameObject::OnTriggerStay(GameObject* other)
{
	if (!m_IsActive) return;

	for (auto& pComponent : m_Components)
	{
		if (pComponent->IsEnabled)
			pComponent->OnTriggerStay(other);
	}
}

void Engine::GameObject::OnTriggerExit(GameObject* other)
{
	if (!m_IsActive) return;

	for (auto& pComponent : m_Components)
	{
		if (pComponent->IsEnabled)
			pComponent->OnTriggerExit(other);
	}
}

void Engine::GameObject::SetActive(bool active)
{
	m_IsActive = active;
}
