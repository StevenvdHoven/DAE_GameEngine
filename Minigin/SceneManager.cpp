#include "SceneManager.h"
#include "Scene.h"

void Engine::SceneManager::Start()
{
	m_pActiveScene->Start();
}

void Engine::SceneManager::Update()
{
	m_pActiveScene->Update();
}

void Engine::SceneManager::FixedUpdate()
{
	m_pActiveScene->FixedUpdate();
}

void Engine::SceneManager::LateUpdate()
{
	m_pActiveScene->LateUpdate();

	if (m_pNextScene != nullptr)
	{
		m_pActiveScene->RemoveAll();
		m_pActiveScene = std::move(m_pNextScene);

		Start();
		m_pNextScene = nullptr;
	}
}

void Engine::SceneManager::Render()
{
	m_pActiveScene->Render();
}

void Engine::SceneManager::SaveScene()
{

}

Engine::Scene* Engine::SceneManager::LoadScene(const std::string& filePath)
{
	return nullptr;
}

Engine::Scene* Engine::SceneManager::CreateScene(const std::string& name)
{
	auto scene = std::make_unique<Scene>(name);
	Scene* rawPtr = scene.get();
	
	if (m_pActiveScene == nullptr)
	{
		m_pActiveScene = std::move(scene);
	}
	else
	{
		m_pNextScene = std::move(scene);
	}

	return rawPtr;
}
