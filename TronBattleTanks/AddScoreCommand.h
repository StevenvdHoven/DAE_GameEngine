#pragma once
#include "Command.h"
#include "ServiceLocator.h"

namespace Engine
{
	class GameObject;
}

class ScoreComponent;
class AddScoreCommand final : public Engine::GameActorCommand
{
public:
	AddScoreCommand(Engine::GameObject* pGameObject);
	virtual ~AddScoreCommand() {};

	void Execute() override;
private:
	ScoreComponent* m_pScoreComponent;
	Engine::SoundClip m_ScoreSound;
};


