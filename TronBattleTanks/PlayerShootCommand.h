#pragma once
#include "Command.h"
#include "GameObject.h"
#include <functional>

namespace Engine
{
	struct Vector2;
}

class PlayerShootCommand : public Engine::GameActorCommand
{
public:
	PlayerShootCommand(Engine::GameObject* pOwner, const Engine::Vector2& offset, std::function<Engine::GameObject* ()> projectileFunction);
	~PlayerShootCommand() override = default;

	void Execute() override;
private:
	Engine::Vector2 m_Offset;
	std::function<Engine::GameObject*()> m_ProjectileFactoryFunction;
};

