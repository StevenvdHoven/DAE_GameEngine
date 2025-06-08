#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include <algorithm>


using namespace Engine;

unsigned int Engine::Scene::m_idCounter = 0;

Engine::Scene::Scene(const std::string& name) : m_name(name), m_Started{ false } {}

void Engine::Scene::Add(std::unique_ptr<GameObject> object)
{
	if (m_Started)
	{
		object->Start();
	}

	m_objects.emplace_back(std::move(object));
}

void Engine::Scene::Remove(GameObject* object)
{
	object->m_IsDestroyed = true;

	for (auto& pComponent : object->m_Components)
	{
		object->RemoveComponent(pComponent.get());
	}
}

void Engine::Scene::RemoveAll()
{
	m_objects.clear();
}

void Engine::Scene::Start()
{
	m_Started = true;

	for (auto& object : m_objects)
	{
		object->Start();
	}
}

void Engine::Scene::Update()
{
	for(auto& object : m_objects)
	{
		object->Update();
	}
}

void Engine::Scene::FixedUpdate()
{
	for (auto& object : m_objects)
	{
		object->FixedUpdate();
	}
}

void Engine::Scene::LateUpdate()
{
	for (auto& object : m_objects)
	{
		object->LateUpdate();
	}

	m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(),
		[](const std::unique_ptr<GameObject>& pGameObject)
		{
			return pGameObject->IsDestroyed();
		}),m_objects.end());
}

void Engine::Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

void Engine::Scene::MoveScene(const Vector2& delta)
{
	for (auto& object : m_objects)
	{
		if (object->GetTransform()->GetParent() != nullptr) continue;

		auto location{ object->GetTransform()->GetLocalPosition() };
		auto futureLocation{ location + delta };
		object->GetTransform()->SetLocalPosition(futureLocation);
	}
}

GameObject* Engine::Scene::FindObjectByName(const std::string& name)
{
	auto it{ std::find_if(m_objects.begin(),m_objects.end(),[&name](std::unique_ptr<GameObject>& gameObject)
		{
			return gameObject->Name() == name;
		}) };
	if (it == m_objects.end()) return nullptr;
	else return (*it).get();
}

nlohmann::json Engine::Scene::Seriliaze() const
{
	nlohmann::json sceneJson;
	sceneJson["scene_name"] = m_name;

	nlohmann::json rootObjectsJson = nlohmann::json::array();
	for (const auto& gameObject : m_objects)
	{
		if (gameObject->GetTransform()->GetParent() != nullptr) continue;

		nlohmann::json goJson;
		gameObject->Serialize(goJson);
		rootObjectsJson.emplace_back(goJson);
	}

	sceneJson["root_game_objects"] = rootObjectsJson;

	return sceneJson;
}

void Engine::Scene::Deserialize(nlohmann::json& in)
{
	RemoveAll();

	m_name = in.value("scene_name", "Unnamed Scene");
	m_Started = false;

	if (!in.contains("root_game_objects"))
		return;

	const auto& rootObjectsJson = in["root_game_objects"];

	for (const auto& goJson : rootObjectsJson)
	{
		std::vector<std::unique_ptr<GameObject>> spawnChildren;
		auto gameObject = std::make_unique<GameObject>();
		gameObject->Deserialize(const_cast<nlohmann::json&>(goJson),spawnChildren);

		Add(std::move(gameObject));
		for (auto& child : spawnChildren)
		{
			m_objects.emplace_back(std::move(child));
		}
	}
}

