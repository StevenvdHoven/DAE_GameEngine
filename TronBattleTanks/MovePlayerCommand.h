#pragma once
#include "Command.h"

namespace Engine
{
	class PhysicsBody;
}

class MovePlayerCommand : public Engine::GameActorCommand2D
{
public:
	MovePlayerCommand(Engine::GameObject* pActor, InputType2D inputType, float speed);
	void Execute(const Engine::Vector2& value) override;
private:
	float m_Speed;
	Engine::PhysicsBody* m_pBody;
};


