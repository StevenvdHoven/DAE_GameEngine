#include "LevelEditor.h"
#include "imgui.h"
#include "SceneManager.h"
#include <fstream>
#include <filesystem>
#include "LevelEditorCommands.h"

#define LEVEL_DIRECTORY "Levels/"

using namespace Engine;

LevelEditor::LevelEditor():
	m_EditingScene{nullptr}
{
}

LevelEditor::~LevelEditor()
{
}

void LevelEditor::Render()
{
	RenderUI();

	if (m_EditingScene)
	{
		m_EditingScene->Render();
	}
}

void LevelEditor::Update()
{
	if (m_EditingScene)
	{
		m_EditingScene->Update();
	}
}

void Engine::LevelEditor::OpenNewScene()
{
	m_EditingScene = SceneManager::GetInstance().CreateScene("New Scene");
}

void LevelEditor::SaveLevel()
{
	auto sceneData{ m_EditingScene->Seriliaze() };
	std::string scenePath{ m_EditingScene->Name() + ".json"};
	std::ofstream file(scenePath);
	if (file.is_open())
	{
		file << sceneData.dump(4);
		file.close();
	}
}

void LevelEditor::LoadLevel(const std::string& filePath)
{
	std::filesystem::path path{ filePath };
	std::string fullPath{ LEVEL_DIRECTORY + filePath };
	std::ifstream file(fullPath);
	if (!file.is_open())
	{
		printf("Couldn't load a level");
		return;
	}

	nlohmann::json sceneData;
	file >> sceneData;

	std::string sceneName{ path.stem().string() };
	auto pScene{ SceneManager::GetInstance().CreateScene(sceneName) };
	pScene->Deserialize(sceneData);

}

void Engine::LevelEditor::RenderUI()
{
	ImGui::Begin("Level Editor");

	char* buffer = new char[256];
	strncpy_s(buffer, 256, m_LevelLoadFilePath.c_str(), m_LevelLoadFilePath.size() + 1);

	if (ImGui::Button("New Scene"))
	{
		OpenNewScene();
	}
	if (ImGui::Button("Save scene"))
	{
		SaveLevel();
	}
	if (ImGui::Button("Load level"))
	{
		LoadLevel(m_LevelLoadFilePath);
	}

	char* buffer = new char[256];
	strncpy_s(buffer, 256, m_EditingScene->Name().c_str(), m_EditingScene->Name().size() + 1);



	ImGui::End();
}
