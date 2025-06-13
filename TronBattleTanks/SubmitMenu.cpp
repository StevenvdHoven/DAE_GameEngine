#include "SubmitMenu.h"
#include "GameObject.h"
#include "Transform.h"
#include "TextRenderer.h"
#include "InputManager.h"
#include "Scoreboard.h"
#include "SDL.h"

using namespace Engine;

SubmitMenu::SubmitMenu(Engine::GameObject* pOwner):
	Engine::Component{pOwner},
	m_Focus{false},
	m_CurrentSelectedText{ 0 },
	m_Score{ 0 },
	m_CharSlots{},
	m_NavigationCommand{ nullptr },
	m_SubmitCommand{ nullptr },
	m_pOnSaved{ std::make_unique<Engine::Subject>() }
{
	LoadCharSlots();
}

SubmitMenu::~SubmitMenu()
{
	TakeFocus();
}

void SubmitMenu::SetScore(int score)
{
	m_Score = score;
	if (!m_pScoreText) return;

	m_pScoreText->SetText(std::to_string(score));

}

void SubmitMenu::OnNavigation(const Engine::Vector2& navigationDirection)
{
	if (!m_Focus) return;

	m_CurrentSelectedText = m_CurrentSelectedText + static_cast<int>( navigationDirection.x);
	if (m_CurrentSelectedText < 0)
	{
		m_CurrentSelectedText = static_cast<int>(m_CharSlots.size()) - 1; 
	}
	else if (m_CurrentSelectedText >= static_cast<int>(m_CharSlots.size()))
	{
		m_CurrentSelectedText = 0; 
	}


	const int length{ static_cast<int>(m_CharSlots.size()) };
	for (int index{ 0 }; index < length; ++index)
	{
		if (index == m_CurrentSelectedText)
		{
			m_CharSlots[index].TextSlot->Color() = Engine::Color{ 255, 0, 0, 255 };
		}
		else
		{
			m_CharSlots[index].TextSlot->Color() = Engine::Color{ 0, 0, 255, 255 };
		}
	}

	if (navigationDirection.y != 0)
	{
		char newChar = static_cast<char>(m_CharSlots[m_CurrentSelectedText].CurrentChar - static_cast<int>(navigationDirection.y));
		if (newChar > 'Z') newChar = 'A';
		else if (newChar < 'A') newChar = 'Z';
		m_CharSlots[m_CurrentSelectedText].CurrentChar = newChar;
		m_CharSlots[m_CurrentSelectedText].TextSlot->SetText(std::string(1, newChar));
	}
}

void SubmitMenu::OnSubmit()
{
	if (!m_Focus) return;

	std::string name;
	for (const auto& slot : m_CharSlots)
	{
		name += slot.CurrentChar;
	}

	if (name.size() < 3)
	{
		return; // Not enough characters to submit
	}

	auto scoreEntries = ScoreBoardReaderWriter::ReadScoreBoard(SCOREBOARD_FILE_PATH);
	ScoreEntry newEntry{ name, m_Score };
	scoreEntries.emplace_back(newEntry);
	std::sort(scoreEntries.begin(), scoreEntries.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
		return a.Score > b.Score; // Sort in descending order
		});
	if (scoreEntries.size() > SCOREBOARD_MAX_ENTRIES) // Limit to top 10 scores
	{
		scoreEntries.resize(SCOREBOARD_MAX_ENTRIES);
	}
	ScoreBoardReaderWriter::WriteScoreBoard(SCOREBOARD_FILE_PATH, scoreEntries);
	m_pOnSaved->Notify(this);
}

void SubmitMenu::GiveFocus()
{
	m_Focus = true;
	m_CurrentSelectedText = 0;

	OnNavigation({});

	auto navigationCommand{ std::make_unique<SubmitMenuNavigation>(this) };
	navigationCommand->ChangeDeviceType(Engine::DeviceType::GAMEPAD);
	navigationCommand->SetInputType(Engine::ValueCommand<Engine::Vector2>::InputType::D_PAD);
	m_NavigationCommand = navigationCommand.get();
	InputManager::GetInstance().Bind2DValue(0, std::move(navigationCommand));

	auto submitCommand{ std::make_unique<SumbitMenuSubmitCommand>(this) };
	submitCommand->ChangeDeviceType(Engine::DeviceType::GAMEPAD);
	submitCommand->SetTriggerState(Engine::TriggerState::PRESSED);
	m_SubmitCommand = submitCommand.get();
	InputManager::GetInstance().BindButton(0, 0x1000, std::move(submitCommand));

	auto keyboardNavigationCommand{ std::make_unique<SubmitMenuNavigation>(this) };
	keyboardNavigationCommand->ChangeDeviceType(Engine::DeviceType::KEYBOARD);
	keyboardNavigationCommand->SetInputType(Engine::ValueCommand<Engine::Vector2>::InputType::ARROW_KEYS);
	m_KeyboardNavigationCommand = keyboardNavigationCommand.get();
	InputManager::GetInstance().Bind2DValue(0, std::move(keyboardNavigationCommand));

	auto keyboardSubmitCommand{ std::make_unique<SumbitMenuSubmitCommand>(this) };
	keyboardSubmitCommand->ChangeDeviceType(Engine::DeviceType::KEYBOARD);
	keyboardSubmitCommand->SetTriggerState(Engine::TriggerState::PRESSED);
	m_KeyboardSumbitCommand = keyboardSubmitCommand.get();
	InputManager::GetInstance().BindButton(0, SDL_SCANCODE_SPACE, std::move(keyboardSubmitCommand));
}

void SubmitMenu::TakeFocus()
{
	m_Focus = false;

	InputManager::GetInstance().Unbind(0, m_NavigationCommand);
	InputManager::GetInstance().Unbind(0, m_SubmitCommand);
	InputManager::GetInstance().Unbind(0, m_KeyboardNavigationCommand);
	InputManager::GetInstance().Unbind(0, m_KeyboardSumbitCommand);
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

	for (size_t i = 0; i < m_CharSlots.size(); ++i)
	{
		m_CharSlots[i].TextSlot->SetText(std::string(1, m_CharSlots[i].CurrentChar));
		m_CharSlots[i].TextSlot->Color() = Engine::Color{ 0, 0, 255, 255 }; // Default color
	}

	m_pScoreText = GetGameObject()->GetTransform()->FindObjectByNameInChilderen("SCORE_TEXT")->GetComponent<Engine::TextRenderer>();
}

std::string SubmitMenu::GetTypeName() const
{
	return "SubmitMenu";
}

SubmitMenuNavigation::SubmitMenuNavigation(SubmitMenu* const menu):
	Engine::ValueCommand<Engine::Vector2>{},
	m_pMenu{menu}
{
}

void SubmitMenuNavigation::Execute(const Engine::Vector2& navigationDirection)
{
	m_pMenu->OnNavigation(navigationDirection);
}

SumbitMenuSubmitCommand::SumbitMenuSubmitCommand(SubmitMenu* const menu):
	Engine::Command{},
	m_pMenu{ menu }
{
}

void SumbitMenuSubmitCommand::Execute()
{
	m_pMenu->OnSubmit();
}

