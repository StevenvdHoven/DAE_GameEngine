#include "GameOverMenuComponent.h"
#include "InputManager.h"
#include "TextRenderer.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "MainMenu.h"
#include "ServiceLocator.h"
#include "Scoreboard.h"
#include "SubmitMenu.h"
#include "SDL.h"

#define DEFAULT_COLOR Engine::Color{0,0,255,255}
#define SELECTED_COLOR Engine::Color{255,0,0,255}

#define PRESS_BUTTON 0x1000

using namespace Engine;

GameOverMenu::NavigationCommand::NavigationCommand(GameOverMenuComponent* const pGameOverMenu):
	Engine::ValueCommand<Engine::Vector2>{},
	m_pGameOverMenu{pGameOverMenu}
{
}

void GameOverMenu::NavigationCommand::Execute(const Engine::Vector2& input)
{
	m_pGameOverMenu->NavigateMenu(input);
}

GameOverMenuComponent::GameOverMenuComponent(Engine::GameObject* pOwner, int score, GameMode mode):
	Engine::Component{pOwner},
	m_HasFocus{ true },
	m_HasSumbitted{false},
	m_Mode{mode},
	m_Score{ score },
	m_CurrentSelectedText{0},
	m_pTextComponents{},
	m_NavigationCommand{nullptr},
	m_ButtonCommand{nullptr},
	m_KeyboardNavigationCommand{nullptr},
	m_KeyboardButtonCommand{nullptr}
{

}

GameOverMenuComponent::~GameOverMenuComponent()
{
	InputManager::GetInstance().Unbind(0, m_ButtonCommand);
	InputManager::GetInstance().Unbind(0, m_NavigationCommand);
	InputManager::GetInstance().Unbind(0, m_KeyboardNavigationCommand);
	InputManager::GetInstance().Unbind(0, m_KeyboardButtonCommand);
}

void GameOverMenuComponent::Start()
{
	auto navigationCommand{ std::make_unique<GameOverMenu::NavigationCommand>(this) };
	navigationCommand->ChangeDeviceType(DeviceType::GAMEPAD);
	navigationCommand->SetInputType(Engine::ValueCommand<Engine::Vector2>::InputType::D_PAD);
	m_NavigationCommand = navigationCommand.get();
	InputManager::GetInstance().Bind2DValue(0, std::move(navigationCommand));

	auto buttonCommand{ std::make_unique<GameOverMenu::PressButtonCommand>(this) };
	buttonCommand->ChangeDeviceType(DeviceType::GAMEPAD);
	buttonCommand->SetTriggerState(TriggerState::PRESSED);
	m_ButtonCommand = buttonCommand.get();
	InputManager::GetInstance().BindButton(0, PRESS_BUTTON, std::move(buttonCommand));

	auto keyboardNavigationCommand{ std::make_unique<GameOverMenu::NavigationCommand>(this) };
	keyboardNavigationCommand->ChangeDeviceType(DeviceType::KEYBOARD);
	keyboardNavigationCommand->SetInputType(Engine::ValueCommand<Engine::Vector2>::InputType::ARROW_KEYS);
	m_KeyboardNavigationCommand = keyboardNavigationCommand.get();
	InputManager::GetInstance().Bind2DValue(0, std::move(keyboardNavigationCommand));

	auto keyboardButtonCommand{ std::make_unique<GameOverMenu::PressButtonCommand>(this) };
	keyboardButtonCommand->ChangeDeviceType(DeviceType::KEYBOARD);
	keyboardButtonCommand->SetTriggerState(TriggerState::PRESSED);
	m_KeyboardButtonCommand = keyboardButtonCommand.get();
	InputManager::GetInstance().BindButton(0, SDL_SCANCODE_SPACE, std::move(keyboardButtonCommand));

	Engine::PrefabResult result{ Engine::EnginePrefabFactory::LoadPrefabs("ScoreSumbit-Parent.json") };
	if (result.bSuccesfull)
	{
		m_SumbitWindow = result.Parent.get();
		m_SubmitComp = m_SumbitWindow->AddComponent<SubmitMenu>();
		m_SubmitComp->SetScore(m_Score);
		m_SubmitComp->GetOnSaved().AddObserver(this);
		SceneManager::GetInstance().GetActiveScene()->Add(std::move(result.Parent));
		auto& childeren{ result.childeren };
		for (auto& child : childeren)
		{
			SceneManager::GetInstance().GetActiveScene()->Add(std::move(child));
		}

		m_SumbitWindow->GetTransform()->SetWorldLocation({ 1000,1000 });
	}

	m_pScoreBoard = SceneManager::GetInstance().GetActiveScene()->FindObjectByName("parent_highscore")->GetComponent<Scoreboard>();

	CreateNavigationTexts();

	NavigateMenu({});
}

void GameOverMenuComponent::NavigateMenu(const Engine::Vector2& direction)
{
	m_CurrentSelectedText += static_cast<int>(direction.y);
	const int length{ static_cast<int>(m_pTextComponents.size()) };
	if (m_CurrentSelectedText >= length) m_CurrentSelectedText = 0;

	for (int index{ 0 }; index < length; ++index)
	{
		if (m_CurrentSelectedText == index)
		{
			m_pTextComponents[index]->Color() = SELECTED_COLOR;
		}
		else
		{
			m_pTextComponents[index]->Color() = DEFAULT_COLOR;
		}
	}
	
}

void GameOverMenuComponent::OnButtonPress()
{
	if (!m_HasFocus) return;

	if (m_CurrentSelectedText == 0)
	{
		MainMenu::CreateScene();
	}
	else if(!m_HasSumbitted)
	{
		m_HasFocus = false;
		m_SumbitWindow->GetTransform()->SetWorldLocation({ 240.0,300.0 });

		m_SubmitComp->GiveFocus();
	}
}

std::string GameOverMenuComponent::GetTypeName() const
{
	return "GameOverMenuComponent";
}

void GameOverMenuComponent::CreateNavigationTexts()
{
	auto pScene{ SceneManager::GetInstance().GetActiveScene() };
	
	auto menuObject{ std::make_unique<Engine::GameObject>() };
	menuObject->GetTransform()->SetLocalPosition({ 50,100 });
	auto menuText{ menuObject->AddComponent<TextRenderer>("Go to Menu","tron-arcade.otf", 20,DEFAULT_COLOR)};
	m_pTextComponents.emplace_back(menuText);
	pScene->Add(std::move(menuObject));

	if (m_Mode != GameMode::VS)
	{
		auto sumbitObject{ std::make_unique<Engine::GameObject>() };
		sumbitObject->GetTransform()->SetLocalPosition({ 50,150 });
		auto sumbitText{ sumbitObject->AddComponent<TextRenderer>("Submit Score","tron-arcade.otf", 20,DEFAULT_COLOR) };
		m_pTextComponents.emplace_back(sumbitText); pScene->Add(std::move(sumbitObject));
	}
}

void GameOverMenuComponent::OnNotify(Component* sender)
{
	if (sender == m_SubmitComp)
	{
		m_HasFocus = true;
		m_SubmitComp->TakeFocus();
		m_SumbitWindow->GetTransform()->SetWorldLocation({ 1000,1000 });
		m_pScoreBoard->Refresh();
		m_CurrentSelectedText = 0;
		NavigateMenu(Engine::Vector2{0,0});
		m_HasSumbitted = true;
	}
}

GameOverMenu::PressButtonCommand::PressButtonCommand(GameOverMenuComponent* const pGameOverMenu):
	Engine::Command{},
	m_pGameOverMenu{pGameOverMenu}
{
}

void GameOverMenu::PressButtonCommand::Execute()
{
	m_pGameOverMenu->OnButtonPress();
}
