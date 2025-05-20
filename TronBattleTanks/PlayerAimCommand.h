#pragma once
#include "Command.h"
namespace Engine
{
	struct Vector2;
}

class PlayerAimCommand : public Engine::GameActorCommand2D
{
public:
	PlayerAimCommand(Engine::GameObject* pOwner);
	~PlayerAimCommand() override = default;
	void Execute(const Engine::Vector2& value) override;
};

