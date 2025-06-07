#pragma once
#include <string>
#include <xmemory>
#include <vector>
#include "GameObject.h"

namespace Engine
{
	class Scene final
	{
		//friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		explicit Scene(const std::string& name);
		~Scene() = default;


		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void Add(std::unique_ptr<GameObject> object);
		void Remove(GameObject* object);
		void RemoveAll();

		void Start();
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;

		void MoveScene(const Vector2& delta);

		nlohmann::json Seriliaze() const;
		void Deserialize(nlohmann::json& out);
		std::string& Name() const { return m_name; }

	private:


		std::string m_name;
		std::vector<std::unique_ptr<GameObject>> m_objects{};
		bool m_Started;

		static unsigned int m_idCounter;
	};
}


