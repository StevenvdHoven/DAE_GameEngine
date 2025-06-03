#include "PlayerAimCommand.h"
#include "Vector2.h"
#include "EngineTime.h"
#include "StaticRotaterComponent.h"

PlayerAimCommand::PlayerAimCommand(Engine::GameObject* pOwner) :
	GameActorCommand2D(pOwner, ValueCommand::InputType2D::RIGHT_STICK),
	pRotator{pOwner->GetComponent<StaticRotaterComponent>()}
{

}

void PlayerAimCommand::Execute(const Engine::Vector2& value)
{
	if (value.SquaredMagnitude() < 0.6f)
		return;

	float rotation{ atan2f(-value.y, value.x) * (180.f / 3.14f) };
	rotation = fmodf(rotation + 360.f, 360.f);
	for (float angle{ 0 }; angle < 360.f; angle += 45.f)
	{
		if (rotation < angle + 45.f)
		{
			rotation = angle;
			break;
		}
	}
	pRotator->SetTargetRotation(rotation);
}



