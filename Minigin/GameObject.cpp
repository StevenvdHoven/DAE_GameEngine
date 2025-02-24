#include <string>
#include "GameObject.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Component.h"

GameObject::GameObject()
{
	m_pTransform = AddComponent<Transform>();
}

GameObject::~GameObject()
{
	
}

void GameObject::RemoveComponent(Component* pComponent)
{
	pComponent->Destroy(pComponent);
}

void GameObject::Start()
{
	for (auto& pComponent : m_Components)
	{
		pComponent->Start();
	}
}

void GameObject::Update()
{
	for(auto& pComponent : m_Components)
	{
		pComponent->Update();
	}
}

void GameObject::FixedUpdate()
{
	for (auto& pComponent : m_Components)
	{
		pComponent->FixedUpdate();
	}
}

void GameObject::LateUpdate()
{
	for (auto& pComponent : m_Components)
	{
		pComponent->LateUpdate();
	}

	m_Components.erase(std::remove_if(m_Components.begin(), m_Components.end(),
			[](const std::unique_ptr<Component>& pComponent)
			{
				return pComponent->IsDestroyed();
			}),m_Components.end());
}

void GameObject::Render() const
{
	for (auto& pComponent : m_Components)
	{
		pComponent->Render();
	}
}
