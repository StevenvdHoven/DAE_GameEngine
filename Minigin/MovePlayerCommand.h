#pragma once
#include "Command.h"

class Transform;
namespace Engine
{
	class MovePlayerCommand : public GameActorCommand2D
	{
	public:
		MovePlayerCommand(GameObject* pActor,InputType2D inputType, float speed);
		void Execute(const Vector2& value) override;
	private:
		float m_Speed;
		Transform* m_pTransform;
	};
}

