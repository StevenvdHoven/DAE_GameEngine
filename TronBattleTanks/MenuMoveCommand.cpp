#include "MenuMoveCommand.h"
#include "MenuComponent.h"

using namespace Engine;

MenuMoveCommand::MenuMoveCommand(Engine::GameObject* pGameObject): 
	GameActorCommand2D{ pGameObject,InputType2D::D_PAD },
	m_pMenuComponent{ pGameObject->GetComponent<MenuComponent>() }
{
}

void MenuMoveCommand::Execute(const Engine::Vector2& input)
{
	if (input.y < -0.1f) m_pMenuComponent->SetGameMode(GameMode::SinglePlayer);
	else if (input.x < -0.1f) m_pMenuComponent->SetGameMode(GameMode::CoOp);
	else if (input.y > 0.1f) m_pMenuComponent->SetGameMode(GameMode::VS);
}
