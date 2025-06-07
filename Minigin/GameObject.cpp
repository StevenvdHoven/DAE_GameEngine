#include <string>
#include "GameObject.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Component.h"
#include "json.hpp"
#include "ServiceLocator.h"

using namespace Engine;

Engine::GameObject::GameObject():
	m_IsActive{true},
	m_IsDestroyed{false},
	m_pTransform{nullptr},
	m_Components{}

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

void Engine::GameObject::Serialize(nlohmann::json& gameObjectJson)
{
	nlohmann::json componentsJson;
	std::vector<std::string> componentTypes;

	for (auto& component : m_Components)
	{
		std::string typeName = component->GetTypeName();
		componentTypes.emplace_back(typeName);

		nlohmann::json compJson;
		component->Serialize(compJson);
		componentsJson[typeName] = compJson;
	}

	gameObjectJson["game_object_component_types"] = componentTypes;
	gameObjectJson["game_object_components"] = componentsJson;

	auto* transform = GetComponent<Engine::Transform>();
	if (transform)
	{
		const auto& children = transform->GetChildren();
		nlohmann::json childrenJson = nlohmann::json::array();

		for (auto* childGO : children)
		{
			nlohmann::json childJson;
			childGO->Serialize(childJson);
			childrenJson.push_back(childJson);
		}

		gameObjectJson["children"] = childrenJson;
	}
}


void Engine::GameObject::Deserialize(nlohmann::json& gameObjectJson)
{
	auto& componentRegistry = ServiceLocator::GetComponentRegistery();

	const std::vector<std::string> typeNames = gameObjectJson["game_object_component_types"];
	const nlohmann::json& componentJson = gameObjectJson["game_object_components"];

	for (const auto& typeName : typeNames)
	{
		componentRegistry.AddComponentByType(this, typeName, componentJson[typeName]);
	}

	if (gameObjectJson.contains("children"))
	{
		auto* transform = GetComponent<Engine::Transform>();
		const auto& childrenJson = gameObjectJson["children"];

		for (const auto& childJson : childrenJson)
		{
			auto childGO = std::make_unique<Engine::GameObject>();
			childGO->Deserialize(const_cast<nlohmann::json&>(childJson)); 

			if (transform)
			{
				childGO->GetComponent<Engine::Transform>()->SetParent(this);
			}
		}
	}
}


void Engine::GameObject::SetActive(bool active)
{
	m_IsActive = active;
}
