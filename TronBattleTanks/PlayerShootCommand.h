#pragma once
#include "Command.h"
#include "GameObject.h"
#include <functional>

namespace Engine
{
	struct Vector2;
}

class PlayerShootCommand : public Engine::GameActorValueCommand<float>
{
public:
	PlayerShootCommand(Engine::GameObject* pOwner, const Engine::Vector2& offset, std::function<Engine::GameObject* ()> projectileFunction);
	~PlayerShootCommand() override = default;

	// Inherited via GameActorValueCommand
	void Execute(const float& value) override;

private:
	Engine::Vector2 m_Offset;
	std::function<Engine::GameObject*()> m_ProjectileFactoryFunction;

	
};

