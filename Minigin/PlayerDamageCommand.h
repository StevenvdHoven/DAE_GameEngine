#pragma once
#include "Command.h"

class PlayerHealthComponent;
namespace Engine
{

	class PlayerDamageCommand final : public GameActorCommand
	{
	public:
		PlayerDamageCommand(GameObject* pActor);
		// Inherited via Command
		void Execute() override;

	private:
		PlayerHealthComponent* m_pHealthComponent;
	};
}



