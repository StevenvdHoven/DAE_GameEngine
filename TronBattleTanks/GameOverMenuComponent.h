#pragma once
#include "Component.h"
#include "Command.h"
#include <vector>
#include "MenuComponent.h"
#include "Observers.h"

namespace Engine
{
	class TextRenderer;
}

class SubmitMenu;
class Scoreboard;

class GameOverMenuComponent final : public Engine::Component, public Engine::IObserver
{
public:
	GameOverMenuComponent(Engine::GameObject* pOwner,int score = 0, GameMode mode = GameMode::SinglePlayer);
	virtual ~GameOverMenuComponent();

	void Start() override;

	void NavigateMenu(const Engine::Vector2& direction);
	void OnButtonPress();

	void OnNotify(Component* sender) override;

	std::string GetTypeName() const override;


private:
	void CreateNavigationTexts();

	GameMode m_Mode;
	bool m_HasFocus;
	bool m_HasSumbitted;
	int m_Score;

	int m_CurrentSelectedText;
	std::vector<Engine::TextRenderer*> m_pTextComponents;
	Engine::GameObject* m_SumbitWindow;
	SubmitMenu* m_SubmitComp;
	Scoreboard* m_pScoreBoard;


	void* m_NavigationCommand;
	void* m_ButtonCommand;

	void* m_KeyboardNavigationCommand;
	void* m_KeyboardButtonCommand;
	
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




