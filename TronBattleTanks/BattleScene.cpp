#include "BattleScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "ImageRenderer.h"
#include "PrefabFactory.h"
#include "Transform.h"
#include "BoxCollider2D.h"
#include "GameLoop.h"
#include "ScoreComponent.h"
#include "StartGameCommand.h"
#include "InputManager.h"
#include "Scene.h"
#include <SDL.h>

using namespace Engine;

void BattleScene::CreateScene()
{
	auto scene{ SceneManager::GetInstance().CreateScene("BattleScene") };

	auto backgroundObject{ std::make_unique<GameObject>() };
	backgroundObject->AddComponent<ImageRenderer>("gameBackground.png");
	scene->Add(std::move(backgroundObject));


	float differenceHeight = 72;
	scene->MoveScene(Vector2{ 0, differenceHeight });

	// Score 
	auto pFont{ ResourceManager::GetInstance().LoadFont("Lingua.otf",32) };
	auto scoreTextObject{ std::make_unique<GameObject>() };
	scoreTextObject->AddComponent<TextRenderer>("",pFont);
	auto scoreComponent{ scoreTextObject->AddComponent<ScoreComponent>() };
	scene->Add(std::move(scoreTextObject));

	auto GameloopObject{ std::make_unique<GameObject>() };
	auto gameloop{ GameloopObject->AddComponent<GameLoop>(scoreComponent) };

	auto startGameCommand{ std::make_unique<StartGameCommand>(gameloop) };
	startGameCommand->ChangeDeviceType(Engine::DeviceType::GAMEPAD);
	startGameCommand->SetTriggerState(Engine::TriggerState::PRESSED);
	InputManager::GetInstance().BindButton(0, 0x010, std::move(startGameCommand)); // Start button

	// For Debug
	auto startGameCommandKeyboard{ std::make_unique<StartGameCommand>(gameloop) };
	startGameCommandKeyboard->ChangeDeviceType(Engine::DeviceType::KEYBOARD);
	startGameCommandKeyboard->SetTriggerState(Engine::TriggerState::PRESSED);
	InputManager::GetInstance().BindButton(0, SDL_SCANCODE_X, std::move(startGameCommandKeyboard)); // Enter button

	scene->Add(std::move(GameloopObject));
}


