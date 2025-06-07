#include "Scene.h"
#include "GameObject.h"

#include <algorithm>
#include "Transform.h"

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

nlohmann::json Engine::Scene::Seriliaze() const
{
	return nlohmann::json();
}

