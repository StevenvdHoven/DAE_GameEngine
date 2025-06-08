#pragma once
#include "Component.h"
#include "Command.h"
#include <vector>
#include "MenuComponent.h"

namespace Engine
{
	class TextRenderer;
}

class SubmitMenu;

class GameOverMenuComponent final : public Engine::Component
{
public:
	GameOverMenuComponent(Engine::GameObject* pOwner, GameMode mode = GameMode::SinglePlayer);
	virtual ~GameOverMenuComponent();

	void Start() override;

	void NavigateMenu(const Engine::Vector2& direction);
	void OnButtonPress();


	std::string GetTypeName() const override;
private:
	void CreateNavigationTexts();

	GameMode m_Mode;
	bool m_HasFocus;

	int m_CurrentSelectedText;
	std::vector<Engine::TextRenderer*> m_pTextComponents;
	Engine::GameObject* m_SumbitWindow;
	SubmitMenu* m_SubmitComp;


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




