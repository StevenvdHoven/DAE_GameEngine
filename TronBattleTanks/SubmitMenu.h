#pragma once
#include "Component.h"
#include "Command.h"
#include <memory>
#include <vector>
#include "Observers.h"

namespace Engine
{
	class TextRenderer;
}

struct CharSlot
{
	Engine::TextRenderer* TextSlot;
	char CurrentChar;
};

class SubmitMenu : public Engine::Component
{
public:
	SubmitMenu(Engine::GameObject* pOwner);
	virtual ~SubmitMenu();

	void SetScore(int score);

	void OnNavigation(const Engine::Vector2& navigationDirection);
	void OnSubmit();

	void GiveFocus();
	void TakeFocus();

	std::string GetTypeName() const override;
	Engine::Subject& GetOnSaved() { return *m_pOnSaved; }
private:
	void LoadCharSlots();

	bool m_Focus;
	int m_CurrentSelectedText;
	int m_Score;
	std::vector<CharSlot> m_CharSlots;
	Engine::TextRenderer* m_pScoreText;

	void* m_NavigationCommand;
	void* m_SubmitCommand;

	void* m_KeyboardNavigationCommand;
	void* m_KeyboardSumbitCommand;

	std::unique_ptr<Engine::Subject> m_pOnSaved;
};

class SubmitMenuNavigation : public Engine::ValueCommand<Engine::Vector2>
{
public:
	SubmitMenuNavigation(SubmitMenu* const menu);

	void Execute(const Engine::Vector2& navigationDirection) override;
private:
	SubmitMenu* const m_pMenu;
};

class SumbitMenuSubmitCommand : public Engine::Command
{
public:
	SumbitMenuSubmitCommand(SubmitMenu* const menu);

	void Execute() override;
private:
	SubmitMenu* const m_pMenu;
};

