#include "StartGameCommand.h"
#include "GameLoop.h"

StartGameCommand::StartGameCommand(GameLoop* pGameLoop):
	Engine::Command(),
	m_pGameLoop{ pGameLoop }
{
}

void StartGameCommand::Execute()
{
	if (m_pGameLoop->GetGameState() == GameState::Running)
		return;

	m_pGameLoop->BeginGame();
}
