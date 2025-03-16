#pragma once
#include "Command.h"

class ScoreComponent;
namespace Engine
{
	class AddScoreCommand final : public GameActorCommand
	{
	public:
		AddScoreCommand(GameObject* pGameObject);
		virtual ~AddScoreCommand() {};

		void Execute() override;
	private:
		ScoreComponent* m_pScoreComponent;
	};
}

