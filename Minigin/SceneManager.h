#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include "Scene.h"

class SceneManager final : public Singleton<SceneManager>
{
public:
	Scene* CreateScene(const std::string& name);

	void Start();
	void Update();
	void FixedUpdate();
	void LateUpdate();
	void Render();

	Scene* GetActiveScene() const { return m_pActiveScene; }
private:
	friend class Singleton<SceneManager>;
	SceneManager() = default;
	Scene* m_pActiveScene{ nullptr };
	std::vector<std::unique_ptr<Scene>> m_Scenes;
};
