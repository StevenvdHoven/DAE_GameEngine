#pragma once
#include "Command.h"

class GameLoop;

class StartGameCommand final : public Engine::Command
{
public:
	StartGameCommand(GameLoop* pGameLoop);

	void Execute() override;

private:
	GameLoop* m_pGameLoop;
};

