#include "PlayerAimCommand.h"
#include "Vector2.h"

PlayerAimCommand::PlayerAimCommand(Engine::GameObject* pOwner) :
	GameActorCommand2D(pOwner, ValueCommand::InputType2D::RIGHT_STICK)
{
}

void PlayerAimCommand::Execute(const Engine::Vector2& value)
{

}



