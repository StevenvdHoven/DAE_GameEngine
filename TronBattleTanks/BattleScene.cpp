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
#include "MenuComponent.h"
#include "Scene.h"
#include <SDL.h>



using namespace Engine;

void BattleScene::CreateScene(GameMode mode = GameMode::SinglePlayer)
{
	auto scene{ SceneManager::GetInstance().CreateScene("BattleScene") };

	float differenceHeight = 72;
	scene->MoveScene(Vector2{ 0, differenceHeight });

	// Score 
	auto pFont{ ResourceManager::GetInstance().LoadFont("tron-arcade.otf",32) };
	auto scoreTextObject{ std::make_unique<GameObject>() };
	scoreTextObject->AddComponent<TextRenderer>("",pFont, Engine::Color{0,0,255,255});
	auto scoreComponent{ scoreTextObject->AddComponent<ScoreComponent>() };
	scene->Add(std::move(scoreTextObject));

	auto GameloopObject{ std::make_unique<GameObject>() };
	GameloopObject->AddComponent<GameLoop>(mode,scoreComponent);

	

	scene->Add(std::move(GameloopObject));
}


