#pragma once
#include "Command.h"

class Transform;
class MovePlayerCommand : public Engine::GameActorCommand2D
{
public:
	MovePlayerCommand(GameObject* pActor, InputType2D inputType, float speed);
	void Execute(const Engine::Vector2& value) override;
private:
	float m_Speed;
	Transform* m_pTransform;
};


