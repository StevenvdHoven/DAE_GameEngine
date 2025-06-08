#include "GameOverMenuComponent.h"
#include "InputManager.h"
#include "TextRenderer.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "MainMenu.h"

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

GameOverMenuComponent::GameOverMenuComponent(Engine::GameObject* pOwner, GameMode mode):
	Engine::Component{pOwner},
	m_Mode{mode},
	m_CurrentSelectedText{0},
	m_pTextComponents{},
	m_NavigationCommand{nullptr}
{

}

GameOverMenuComponent::~GameOverMenuComponent()
{
	InputManager::GetInstance().Unbind(0, m_ButtonCommand);
	InputManager::GetInstance().Unbind(0, m_NavigationCommand);
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
	buttonCommand->SetTriggerState(TriggerState::RELEASED);
	m_ButtonCommand = buttonCommand.get();
	InputManager::GetInstance().BindButton(0, PRESS_BUTTON, std::move(buttonCommand));

	CreateNavigationTexts();

	NavigateMenu({});
}

void GameOverMenuComponent::NavigateMenu(const Engine::Vector2& direction)
{
	m_CurrentSelectedText += static_cast<int>(direction.y);
	if (m_CurrentSelectedText >= m_pTextComponents.size()) m_CurrentSelectedText = 0;

	for (int index{ 0 }; index < m_pTextComponents.size(); ++index)
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
	if (m_CurrentSelectedText == 0)
	{
		MainMenu::CreateScene();
	}
	else
	{

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

GameOverMenu::PressButtonCommand::PressButtonCommand(GameOverMenuComponent* const pGameOverMenu):
	Engine::Command{},
	m_pGameOverMenu{pGameOverMenu}
{
}

void GameOverMenu::PressButtonCommand::Execute()
{
	m_pGameOverMenu->OnButtonPress();
}
