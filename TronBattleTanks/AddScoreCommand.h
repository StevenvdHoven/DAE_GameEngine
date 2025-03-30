#pragma once
#include "Command.h"

class ScoreComponent;
class AddScoreCommand final : public Engine::GameActorCommand
{
public:
	AddScoreCommand(GameObject* pGameObject);
	virtual ~AddScoreCommand() {};

	void Execute() override;
private:
	ScoreComponent* m_pScoreComponent;
};


