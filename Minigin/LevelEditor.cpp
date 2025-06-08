#include "LevelEditor.h"
#include "imgui.h"
#include "SceneManager.h"
#include <fstream>
#include <filesystem>
#include "LevelEditorCommands.h"
#include "ImageRenderer.h"
#include "InputManager.h"
#include "BoxCollider2D.h"
#include <memory>
#include <SDL.h>

#define LEVEL_DIRECTORY "../Data/Levels/"
#define PREFAB_FILEPATH "../Data/Prefabs/"

using namespace Engine;

LevelEditor::LevelEditor():
	m_Active{false},
	m_LevelLoadFilePath{ "Level1.json" },
	m_EditingScene{nullptr},
	m_SelectedGameObject{ nullptr }
{	
	auto toggleCommand{ std::make_unique<ToggleLevelEditorCommand>() };
	InputManager::GetInstance().BindButton(0, SDL_SCANCODE_F2, std::move(toggleCommand));
}

LevelEditor::~LevelEditor()
{
}

void Engine::LevelEditor::GUI()
{
	ImGui::Begin("Level Editor");

	char* buffer = new char[256];
	strncpy_s(buffer, 256, m_LevelLoadFilePath.c_str(), m_LevelLoadFilePath.size() + 1);
	if (ImGui::InputText("Level File Path", buffer, 256))
	{
		m_LevelLoadFilePath = std::string(buffer);
	}

	ImGui::SeparatorText("--  Scene -- ");

	if (ImGui::Button("New Scene"))
	{
		OpenNewScene();
		ImGui::End();
		return;
	}
	if (ImGui::Button("Save scene"))
	{
		SaveLevel();
	}
	if (ImGui::Button("Load level"))
	{
		if (m_EditingScene)
		{
			m_EditingScene->RemoveAll();
		}

		m_SelectedGameObject = nullptr;
		m_EditingScene = LoadLevel(m_LevelLoadFilePath);
		ImGui::End();
		return;

	}

	ImGui::SeparatorText("-- Scene Settings --");

	buffer = new char[256];
	strncpy_s(buffer, 256, m_EditingScene->Name().c_str(), m_EditingScene->Name().size() + 1);
	if (ImGui::InputText("Scene Name", buffer, 256))
	{
		m_EditingScene->Name() =std::string(buffer);
	}

	if (ImGui::Button("Create GameObject"))
	{
		CreateGameObject();
	}
	ImGuiScene();

	ImGui::End();

	ImGuiSelectedGameObject();
}

void LevelEditor::Render()
{
	
}

void LevelEditor::Update()
{

}

void Engine::LevelEditor::LateUpdate()
{

}

void Engine::LevelEditor::OpenNewScene()
{
	if (m_EditingScene)
	{
		m_EditingScene->RemoveAll();
	}
	m_SelectedGameObject = nullptr;

	m_EditingScene = SceneManager::GetInstance().CreateScene("New Scene");
}

void LevelEditor::SaveLevel()
{
	auto sceneData{ m_EditingScene->Seriliaze() };
	std::string scenePath{ LEVEL_DIRECTORY + m_EditingScene->Name() + ".json"};
	std::ofstream file(scenePath);
	if (file.is_open())
	{
		file << sceneData.dump(4);
		file.close();
	}
}

Scene* LevelEditor::LoadLevel(const std::string& filePath)
{
	std::filesystem::path path{ filePath };
	std::string fullPath{ LEVEL_DIRECTORY + filePath };
	std::ifstream file(fullPath);
	if (!file.is_open())
	{
		printf("Couldn't load a level");
		return SceneManager::GetInstance().CreateScene("Failed scene");
	}

	nlohmann::json sceneData;
	file >> sceneData;

	std::string sceneName{ path.stem().string() };
	auto scene = SceneManager::GetInstance().CreateScene(sceneName);
	scene->Deserialize(sceneData);
	return scene;
}

void Engine::LevelEditor::SetLevel(Scene* const pScene)
{
	m_EditingScene = pScene;
}

void Engine::LevelEditor::CreateGameObject()
{
	auto newGameObject{ std::make_unique<GameObject>() };
	newGameObject->AddComponent<ImageRenderer>();
	newGameObject->AddComponent<BoxCollider2D>();
	m_SelectedGameObject = newGameObject.get();
	m_EditingScene->Add(std::move(newGameObject));
	
}

void Engine::LevelEditor::SetActive(bool active)
{
	m_Active = active;
}

bool Engine::LevelEditor::IsActive() const
{
	return m_Active;
}

void Engine::LevelEditor::ImGuiSelectedGameObject()
{
	if (m_SelectedGameObject)
	{
		ImGui::Begin("Inspector GameObject");
		m_SelectedGameObject->GUI();
		ImGui::End();
	}
}

void Engine::LevelEditor::ImGuiScene()
{
	auto& allObjects{ m_EditingScene->GetAllObjects() };

	for (auto& gameObject : allObjects)
	{
		if (gameObject->GetTransform()->GetParent() != nullptr)
			continue;

		if (gameObject->PreviewGUI(m_SelectedGameObject))
		{
			m_SelectedGameObject = nullptr;
			return;
		}
	}
}

Engine::PrefabResult Engine::EnginePrefabFactory::LoadPrefabs(const std::string& prefabPath)
{
	Engine::PrefabResult result{};

	std::string fullPath{ PREFAB_FILEPATH + prefabPath };
	std::ifstream file(fullPath);
	if (!file.is_open())
	{
		printf("Couldn't load prefab");
		return result;
	}

	nlohmann::json prefabData;
	file >> prefabData;

	result.Parent = std::make_unique<Engine::GameObject>();
	result.Parent->Deserialize(prefabData, result.childeren);
	result.bSuccesfull = true;
	return result;
}

void Engine::EnginePrefabFactory::SavePrefab(Engine::GameObject* gameObject)
{
	nlohmann::json json;
	gameObject->Serialize(json);
	std::string scenePath{ PREFAB_FILEPATH + gameObject->Name() + ".json" };
	std::ofstream file(scenePath);
	if (file.is_open())
	{
		file << json.dump(4);
		file.close();
	}
}


void Engine::EnginePrefabFactory::AddPrefabToScene(Engine::PrefabResult&& result, Scene* const pScene)
{
	if (!result.bSuccesfull || !pScene)
	{
		return;
	}

	pScene->Add(std::move(result.Parent));
	for (auto& child : result.childeren)
	{
		pScene->Add(std::move(child));
	}
}
