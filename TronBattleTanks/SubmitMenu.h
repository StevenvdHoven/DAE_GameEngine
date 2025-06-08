#pragma once
#include "Component.h"
#include "Command.h"
#include <vector>

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

	void OnNavigation(const Engine::Vector2& navigationDirection);

	void GiveFocus() { m_Focus = true; }
	void TakeFocus() { m_Focus = false; }

private:
	void LoadCharSlots();

	bool m_Focus;
	std::vector<CharSlot> m_CharSlots;
};

class SubmitMenuNavigation : public Engine::ValueCommand<Engine::Vector2>
{
public:
	SubmitMenuNavigation(SubmitMenu* const menu);

	void Execute(const Engine::Vector2& navigationDirection) override;
private:
	SubmitMenu* const m_pMenu;
};

