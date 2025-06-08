#include "SubmitMenu.h"
#include "GameObject.h"
#include "Transform.h"
#include "TextRenderer.h"

SubmitMenu::SubmitMenu(Engine::GameObject* pOwner):
	Engine::Component{pOwner},
	m_Focus{false},
	m_CharSlots{}
{
	LoadCharSlots();
}

void SubmitMenu::OnNavigation(const Engine::Vector2&)
{
}

void SubmitMenu::LoadCharSlots()
{
	m_CharSlots.resize(3);
	m_CharSlots[0] = CharSlot
	{
		GetGameObject()->GetTransform()->FindObjectByNameInChilderen("FIRST_LETTER")->GetComponent<Engine::TextRenderer>(),
		'A'
	};
	m_CharSlots[1] = CharSlot
	{
		GetGameObject()->GetTransform()->FindObjectByNameInChilderen("SECOND_LETTER")->GetComponent<Engine::TextRenderer>(),
		'A'
	};
	m_CharSlots[2] = CharSlot
	{
		GetGameObject()->GetTransform()->FindObjectByNameInChilderen("THIRD_LETTER")->GetComponent<Engine::TextRenderer>(),
		'A'
	};
}

SubmitMenuNavigation::SubmitMenuNavigation(SubmitMenu* const menu):
	Engine::ValueCommand<Engine::Vector2>{},
	m_pMenu{menu}
{
}

void SubmitMenuNavigation::Execute(const Engine::Vector2&)
{

}
