#include "LevelEditorCommands.h"
#include "ServiceLocator.h"
#include "SceneManager.h"

void Engine::ToggleLevelEditorCommand::Execute()
{
	ServiceLocator::GetLevelEditor().SetActive(!ServiceLocator::GetLevelEditor().IsActive());

	if (ServiceLocator::GetLevelEditor().IsActive())
	{
		ServiceLocator::GetLevelEditor().SetLevel(SceneManager::GetInstance().GetActiveScene());
	}
}

void Engine::CreateGameObjectCommand::Execute()
{
	if (ServiceLocator::GetLevelEditor().IsActive())
		ServiceLocator::GetLevelEditor().CreateGameObject();
}
