#include "MenuComponent.h"
#include "SceneManager.h"
#include "MenuMoveCommand.h"
#include "ImageRenderer.h"
#include "InputManager.h"
#include "EngineTime.h"
#include "GameObject.h"
#include "BattleScene.h"
#include <memory>

using namespace Engine;

MenuComponent::MenuComponent(Engine::GameObject* pGameObject): 
	Component{ pGameObject },
	m_pMenuPlayer{ nullptr },
	m_GameModeSelected{ false },
	m_TransitionTime{ 0.0f },
	m_MenuPlayerStartLocation{ 240.0f, 340.0f },
	m_MenuPlayerTargetLocation{ 0.0f, 0.0f },
	m_SelectedGameMode{ GameMode::SinglePlayer }
{
	

}

void MenuComponent::Start()
{
	auto pGameObject = GetGameObject();
	// Register the command to handle input for menu navigation
	auto moveCommand = std::make_unique<MenuMoveCommand>(pGameObject);
	InputManager::GetInstance().Bind2DValue(0, std::move(moveCommand));

	CreateMenuPlayer();
}

void MenuComponent::Update()
{
	if (!m_GameModeSelected) return;

	if (m_TransitionTime <= 1.0f)
	{
		m_TransitionTime += Time::GetInstance().GetDeltaTime();
		
		Vector2 lerpedPosition = Vector2::Lerp(m_MenuPlayerStartLocation, m_MenuPlayerTargetLocation, m_TransitionTime);
		m_pMenuPlayer->GetTransform()->SetWorldLocation(lerpedPosition);
	}
	else
	{
		BattleScene::CreateScene(m_SelectedGameMode);
	}
	
}

void MenuComponent::SetGameMode(GameMode mode)
{
	if (m_GameModeSelected) return;

	constexpr float transitionDistance{ 100.0f };

	m_SelectedGameMode = mode;
	m_GameModeSelected = true;

	Vector2 center = { 240.0f, 320.0f };
	switch (mode)
	{
	case GameMode::SinglePlayer:
		m_MenuPlayerTargetLocation = center + Vector2{ 0.0f, -transitionDistance };
		printf("Single Player selected\n");
		break;
	case GameMode::CoOp:
		m_MenuPlayerTargetLocation = center + Vector2{ -transitionDistance, 0.0f };
		printf("Co-Op selected\n");
		break;
	case GameMode::VS:
		m_MenuPlayerTargetLocation = center + Vector2{ 0.0f, transitionDistance };
		printf("1v1 selected\n");
		break;
	}
}

void MenuComponent::CreateMenuPlayer()
{
	auto scene = SceneManager::GetInstance().GetActiveScene();

	auto pMenuPlayer{ std::make_unique<GameObject>() };
	auto pImageRenderer{ pMenuPlayer->AddComponent<ImageRenderer>("Menu_player.png") };
	pImageRenderer->ChangeImageAllignment(ImageAllignment::Centre);
	pMenuPlayer->GetTransform()->SetWorldLocation(m_MenuPlayerStartLocation);


	m_pMenuPlayer = pMenuPlayer.get();
	scene->Add(std::move(pMenuPlayer));


}
