#pragma once
#include "Command.h"

class PlayerHealthComponent;
class PlayerDamageCommand final : public Engine::GameActorCommand
{
public:
	PlayerDamageCommand(Engine::GameObject* pActor);
	virtual ~PlayerDamageCommand() {};
	// Inherited via Command
	void Execute() override;

private:
	PlayerHealthComponent* m_pHealthComponent;
};




