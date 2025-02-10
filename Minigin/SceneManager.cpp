#include "SceneManager.h"
#include "Scene.h"

void SceneManager::Start()
{
	for (auto& scene : m_Scenes)
	{
		scene->Start();
	}
}

void SceneManager::Update()
{
	for(auto& scene : m_Scenes)
	{
		scene->Update();
	}
}

void SceneManager::FixedUpdate()
{
	for (auto& scene : m_Scenes)
	{
		scene->FixedUpdate();
	}
}

void SceneManager::Render()
{
	for (const auto& scene : m_Scenes)
	{
		scene->Render();
	}
}

Scene* SceneManager::CreateScene(const std::string& name)
{
	auto scene = std::make_unique<Scene>(name);
	Scene* rawPtr = scene.get();
	m_Scenes.push_back(std::move(scene));
	return rawPtr;
}
