#pragma once
#include "Command.h"
#include <functional>

namespace Engine
{
	struct Vector2;
}


class EnemyShootCommand final : public Engine::GameActorCommand
{
public:
	EnemyShootCommand(Engine::GameObject* pActor,const Engine::Vector2& offset, std::function<Engine::GameObject* ()> projectileFunction);
	virtual ~EnemyShootCommand() = default;

	void Execute() override;

private:
	const std::function<Engine::GameObject* ()> m_ProjectileFunction;
	const Engine::Vector2 m_Offset;
};

