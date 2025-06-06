#pragma once
#include "Component.h"
#include "Command.h"
#include <vector>

namespace Engine
{
	class TextRenderer;
}

class GameOverMenuComponent final : public Engine::Component
{
public:
	GameOverMenuComponent(Engine::GameObject* pOwner);
	virtual ~GameOverMenuComponent();

	void Start() override;

	void NavigateMenu(const Engine::Vector2& direction);
	void OnButtonPress();

private:
	void CreateNavigationTexts();

	int m_CurrentSelectedText;
	std::vector<Engine::TextRenderer*> m_pTextComponents;


	void* m_NavigationCommand;
	void* m_ButtonCommand;
};

namespace GameOverMenu
{
	class NavigationCommand final : public Engine::ValueCommand<Engine::Vector2>
	{
	public:
		NavigationCommand(GameOverMenuComponent* const pGameOverMenu);

		void Execute(const Engine::Vector2& input) override;
	private:
		GameOverMenuComponent* const m_pGameOverMenu;
	};

	class PressButtonCommand final : public Engine::Command
	{
	public:
		PressButtonCommand(GameOverMenuComponent* const pGameOverMenu);

		void Execute() override;
	private:
		GameOverMenuComponent* const m_pGameOverMenu;
	};
}




