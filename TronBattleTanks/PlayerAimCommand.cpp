#include "PlayerAimCommand.h"
#include "Vector2.h"
#include "EngineTime.h"


PlayerAimCommand::PlayerAimCommand(Engine::GameObject* pOwner) :
	GameActorCommand2D(pOwner, ValueCommand::InputType2D::RIGHT_STICK)
{

}

void PlayerAimCommand::Execute(const Engine::Vector2& value)
{
	if (value.SquaredMagnitude() < 0.01f)
		return;

	float rotation{ atan2f(-value.y, value.x) * (180.f / 3.14f) };
	rotation = fmodf(rotation + 360.f, 360.f);
	auto pTransform{ GetActor()->GetTransform() };
	pTransform->SetWorldRotation(rotation);
}



