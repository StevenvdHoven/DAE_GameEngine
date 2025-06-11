#include "MenuComponent.h"
#include "SceneManager.h"
#include "MenuMoveCommand.h"
#include "ImageRenderer.h"
#include "InputManager.h"
#include "EngineTime.h"
#include "GameObject.h"
#include "BattleScene.h"
#include "SkipSceneCommand.h"
#include "ServiceLocator.h"
#include <memory>
#include <SDL.h>

using namespace Engine;

MenuComponent::MenuComponent(Engine::GameObject* pGameObject) :
	Component{ pGameObject },
	m_pMenuPlayer{ nullptr },
	m_GameModeSelected{ false },
	m_TransitionTime{ 0.0f },
	m_MenuPlayerStartLocation{ 240.0f, 340.0f },
	m_MenuPlayerTargetLocation{ 0.0f, 0.0f },
	m_SelectedGameMode{ GameMode::SinglePlayer },
	m_IntroSound{ServiceLocator::GetSoundSystem().LoadSound("tron_intro.wav")},
	m_MenuMoveCommand{nullptr}
{
}

void MenuComponent::Start()
{
	auto pGameObject = GetGameObject();
	// Register the command to handle input for menu navigation
	auto moveCommand = std::make_unique<MenuMoveCommand>(pGameObject);
	m_MenuMoveCommand = moveCommand.get();

	auto keyboardMoveCommand{ std::make_unique<MenuMoveCommand>(pGameObject) };
	keyboardMoveCommand->ChangeDeviceType(DeviceType::KEYBOARD);
	keyboardMoveCommand->SetInputType(ValueCommand<Engine::Vector2>::InputType::ARROW_KEYS);
	m_MenuKeyboardMoveCommand = keyboardMoveCommand.get();

	auto skipSceneCommand{ std::make_unique<SkipSceneCommand>([]()
		{
			BattleScene::CreateScene(GameMode::SinglePlayer);
		}) };
	skipSceneCommand->ChangeDeviceType(DeviceType::KEYBOARD);
	skipSceneCommand->SetTriggerState(TriggerState::PRESSED);
	m_SkipSceneCommand = skipSceneCommand.get();


	InputManager::GetInstance().Bind2DValue(0, std::move(moveCommand));
	InputManager::GetInstance().Bind2DValue(0, std::move(keyboardMoveCommand));
	InputManager::GetInstance().BindButton(0,SDL_SCANCODE_F1, std::move(skipSceneCommand));

	CreateMenuPlayer();

	ServiceLocator::GetSoundSystem().PlaySound(m_IntroSound);
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
		InputManager::GetInstance().Unbind(0, m_MenuMoveCommand);
		InputManager::GetInstance().Unbind(0, m_MenuKeyboardMoveCommand);
		InputManager::GetInstance().Unbind(0, m_SkipSceneCommand);
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

std::string MenuComponent::GetTypeName() const
{
	return "MenuComponent";
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
