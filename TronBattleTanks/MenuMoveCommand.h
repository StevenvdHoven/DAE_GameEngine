#pragma once
#include "Command.h"

class MenuComponent;

class MenuMoveCommand : public Engine::GameActorCommand2D
{
public:
	MenuMoveCommand(Engine::GameObject* pGameObject);

	void Execute(const Engine::Vector2& input);

private:
	MenuComponent* m_pMenuComponent;
};

