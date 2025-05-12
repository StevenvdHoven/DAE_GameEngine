#pragma once
#include "Command.h"
#include "GameLoop.h"


class StartGameCommand final : public Engine::Command
{
public:
	StartGameCommand(GameLoop* pGameLoop);

	void Execute() override;

private:
	GameLoop* m_pGameLoop;
};

