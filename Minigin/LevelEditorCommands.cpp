#include "LevelEditorCommands.h"
#include "ServiceLocator.h"

void Engine::ToggleLevelEditorCommand::Execute()
{
	ServiceLocator::GetLevelEditor().SetActive(!ServiceLocator::GetLevelEditor().IsActive());
}

void Engine::CreateGameObjectCommand::Execute()
{

}
