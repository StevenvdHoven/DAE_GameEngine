#include "GrapEditorCommands.h"
#include "ServiceLocator.h"

void Engine::OpenGraphEditor::Execute()
{
	auto& graphEditor = ServiceLocator::GetGraphEditor();
	bool isActive = graphEditor.IsActive();
	if (isActive)
	{
		graphEditor.SetActive(false);
	}
	else
	{
		graphEditor.SetActive(true);
	}
}

void Engine::AddGraphNodeCommand::Execute()
{
	auto& graphEditor = ServiceLocator::GetGraphEditor();
	graphEditor.CreateNode();
}

void Engine::DeleteGraphNodeCommand::Execute()
{
	auto& graphEditor = ServiceLocator::GetGraphEditor();
	graphEditor.DeleteNode();
}

void Engine::SaveGraphCommand::Execute()
{
	auto& graphEditor = ServiceLocator::GetGraphEditor();
	graphEditor.SaveGraph();
}
