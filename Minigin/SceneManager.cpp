#include "SceneManager.h"
#include "Scene.h"

void Engine::SceneManager::Start()
{
	for (auto& scene : m_Scenes)
	{
		scene->Start();
	}
}

void Engine::SceneManager::Update()
{
	for(auto& scene : m_Scenes)
	{
		scene->Update();
	}
}

void Engine::SceneManager::FixedUpdate()
{
	for (auto& scene : m_Scenes)
	{
		scene->FixedUpdate();
	}
}

void Engine::SceneManager::LateUpdate()
{
	for (auto& scene : m_Scenes)
	{
		scene->LateUpdate();
	}
}

void Engine::SceneManager::Render()
{
	for (const auto& scene : m_Scenes)
	{
		scene->Render();
	}
}

Engine::Scene* Engine::SceneManager::CreateScene(const std::string& name)
{
	

	auto scene = std::make_unique<Scene>(name);
	Scene* rawPtr = scene.get();
	m_Scenes.push_back(std::move(scene));

	if (m_pActiveScene == nullptr)
	{
		m_pActiveScene = rawPtr;
	}

	return rawPtr;
}
