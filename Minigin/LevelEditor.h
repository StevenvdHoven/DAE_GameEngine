#pragma once
#include <string>

namespace Engine
{
	class Scene;
	class GameObject;


	class LevelEditor final
	{
	public:
		LevelEditor();
		~LevelEditor();

		void GUI();
		void Render();
		void Update();
		void LateUpdate();

		void OpenNewScene();
		void SaveLevel();
		Scene* LoadLevel(const std::string& filePath);
		void SetLevel(Scene* const pScene);

		void CreateGameObject();

		void SetActive(bool active);
		bool IsActive() const;
			
	private:
		void ImGuiSelectedGameObject();
		void ImGuiScene();

		bool m_Active;
		std::string m_LevelLoadFilePath;
		Scene* m_EditingScene;
		GameObject* m_SelectedGameObject;
	};

}