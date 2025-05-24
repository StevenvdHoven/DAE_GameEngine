#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include "Scene.h"

namespace Engine
{
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		SceneManager() = default;
		Scene* CreateScene(const std::string& name);

		void Start();
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render();

		Scene* GetActiveScene() const { return m_pActiveScene.get(); }
	private:
		
		std::unique_ptr<Scene> m_pActiveScene{ nullptr };
		std::unique_ptr<Scene> m_pNextScene{ nullptr };
	};
}
