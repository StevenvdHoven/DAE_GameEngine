#pragma once
#include <string>

namespace Engine
{
	class Scene;


	class LevelEditor final
	{
	public:
		LevelEditor();
		~LevelEditor();

		void Render();
		void Update();

		void OpenNewScene();
		void SaveLevel();
		void LoadLevel(const std::string& filePath);

		void SetActive(bool active) { m_Active = active; }
		bool IsActive() const { return m_Active; }
			
	private:
		void RenderUI();

		bool m_Active;
		std::string m_LevelLoadFilePath;
		Scene* m_EditingScene;
	};

}