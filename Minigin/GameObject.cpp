#include <string>
#include "GameObject.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Component.h"

GameObject::GameObject()
{
	auto transform{ std::make_unique<Transform>() };
	m_pTransform = transform.get();
	AddComponent(std::move(transform));
}

GameObject::~GameObject() = default;

void GameObject::AddComponent(std::unique_ptr<Component> pComponent)
{
	pComponent->m_pGameObject = this;
	pComponent->m_pTransform = m_pTransform;

	m_Components.emplace_back(std::move(pComponent));
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

void GameObject::Render() const
{
	for (auto& pComponent : m_Components)
	{
		pComponent->Render();
	}
}
