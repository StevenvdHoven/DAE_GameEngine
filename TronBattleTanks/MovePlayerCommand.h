#pragma once
#include "Command.h"

namespace Engine
{
	class PhysicsBody;
	class Collider;
}

class PlayerMovement;

class MovePlayerCommand : public Engine::GameActorCommand2D
{
public:
	MovePlayerCommand(Engine::GameObject* pActor, InputType inputType);
	void Execute(const Engine::Vector2& value) override;
private:
	float m_Speed;
	PlayerMovement* m_pPlayerMovement;

};


