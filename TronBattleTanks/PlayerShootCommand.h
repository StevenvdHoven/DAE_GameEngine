#pragma once
#include "Command.h"
#include "GameObject.h"
#include <functional>
class PlayerShootCommand : public Engine::GameActorCommand
{
public:
	PlayerShootCommand(Engine::GameObject* pOwner, std::function<Engine::GameObject*()> projectileFunction);
	~PlayerShootCommand() override = default;

	void Execute() override;
private:
	std::function<Engine::GameObject*()> m_ProjectileFactoryFunction;
};

