#include <string>
#include "GameObject.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Component.h"
#include "json.hpp"
#include "ServiceLocator.h"
#include "SceneManager.h"
#include "imgui.h"
#include "json.hpp"

using namespace Engine;

Engine::GameObject::GameObject(const std::string& name) :
	m_Name{ name },
	m_IsActive{ true },
	m_IsDestroyed{ false },
	m_Components{},
	m_pTransform{ AddComponent<Transform>() }
{
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
	gameObjectJson["game_object_name"] = m_Name;
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


void Engine::GameObject::Deserialize(nlohmann::json& gameObjectJson, std::vector<std::unique_ptr<GameObject>>& spawnChilderen)
{
	auto& componentRegistry = ServiceLocator::GetComponentRegistery();

	m_Name = gameObjectJson["game_object_name"].get<std::string>();
	const std::vector<std::string> typeNames = gameObjectJson["game_object_component_types"];
	const nlohmann::json& componentJson = gameObjectJson["game_object_components"];

	for (const auto& typeName : typeNames)
	{
		if (typeName == "TransformComponent")
		{
			m_pTransform->Deserialize(componentJson[typeName]);
			continue;
		}
		componentRegistry.AddComponentByType(this, typeName, componentJson[typeName]);
	}

	if (gameObjectJson.contains("children"))
	{
		const auto& childrenJson = gameObjectJson["children"];

		for (const auto& childJson : childrenJson)
		{
			auto childGO = std::make_unique<Engine::GameObject>();
			childGO->GetTransform()->SetParent(this);
			auto rawPtr{ childGO.get() };
			spawnChilderen.emplace_back(std::move(childGO));
			rawPtr->Deserialize(const_cast<nlohmann::json&>(childJson), spawnChilderen);
		}
	}
}


void Engine::GameObject::SetActive(bool active)
{
	m_IsActive = active;
}

bool Engine::GameObject::PreviewGUI(GameObject*& selectedObject)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (selectedObject == this)
		flags |= ImGuiTreeNodeFlags_Selected;

	if (m_pTransform->GetChildren().empty())
		flags |= ImGuiTreeNodeFlags_Leaf;

	ImGui::PushID(this);

	bool nodeOpen = ImGui::TreeNodeEx(m_Name.c_str(), flags);

	if (ImGui::IsItemClicked())
	{
		selectedObject = this;
	}

	if (ImGui::IsItemHovered())
	{
		const auto pos = m_pTransform->GetWorldLocation();
		ImGui::SetTooltip("Position: (%.1f, %.1f)", pos.x, pos.y);
	}

	ImGui::SameLine();
	if (ImGui::SmallButton("+"))
	{
		auto newGameObject{ std::make_unique<GameObject>("New GameObject") };
		newGameObject->GetTransform()->SetParent(this);
		SceneManager::GetInstance().GetActiveScene()->Add(std::move(newGameObject));
		ImGui::TreePop();
		ImGui::PopID();
		return true;
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("-"))
	{
		SceneManager::GetInstance().GetActiveScene()->Remove(this);
		ImGui::TreePop();
		ImGui::PopID();
		return true;
	}

	if (nodeOpen)
	{
		for (auto& child : m_pTransform->GetChildren())
		{
			if (child->PreviewGUI(selectedObject))
			{
				ImGui::TreePop();
				ImGui::PopID();
				return true;
			}
		}
		ImGui::TreePop();
	}

	ImGui::PopID();
	return false;
}

void Engine::GameObject::GUI()
{
	// --- Editable Name ---
	char buffer[256];
	strncpy_s(buffer, m_Name.c_str(), sizeof(buffer));
	buffer[sizeof(buffer) - 1] = '\0';

	if (ImGui::InputText("Name", buffer, sizeof(buffer)))
	{
		m_Name = std::string(buffer);
	}

	ImGui::Separator();

	// --- Add Component Dropdown ---
	static std::string selectedToAdd;
	const auto& registeredTypes = ServiceLocator::GetComponentRegistery().GetRegisteredTypes();

	if (ImGui::BeginCombo("Add Component", selectedToAdd.empty() ? "Select Component..." : selectedToAdd.c_str()))
	{
		for (const auto& type : registeredTypes)
		{
			if (ImGui::Selectable(type.c_str()))
			{
				selectedToAdd = type;
				if (!HasComponent(type))
				{
					ServiceLocator::GetComponentRegistery().AddComponentByType(this, type, nlohmann::json{});
				}
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Separator();

	// --- Component GUIs ---
	for (auto& component : m_Components)
	{
		ImGui::PushID(component.get());
		std::string headerLabel = "Component: " + component->GetTypeName();
		if (ImGui::CollapsingHeader(headerLabel.c_str(), ImGuiTreeNodeFlags_CollapsingHeader))
		{
			if (ImGui::Button("Delete"))
			{
				component->Destroy(component.get());
				ImGui::PopID();
				return;
			}
			component->GUI();
		}
		ImGui::PopID();
	}
}
