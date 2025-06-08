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
#include "ServiceLocator.h"
#include "Scene.h"
#include <SDL.h>



using namespace Engine;

void BattleScene::CreateScene(GameMode mode = GameMode::SinglePlayer)
{
	auto scene{ ServiceLocator::GetLevelEditor().LoadLevel("Map01.json")};

	// Score 
	std::unique_ptr<Engine::GameObject> scoreTextObject;

	if (mode != GameMode::VS)
	{
		scoreTextObject = std::make_unique<GameObject>();
		scoreTextObject->AddComponent<TextRenderer>("Score", "tron-arcade.otf", 32, Engine::Color{ 0,0,255,255 });
		scene->Add(std::move(scoreTextObject));
	}

	scoreTextObject = std::make_unique<GameObject>();
	scoreTextObject->GetTransform()->SetLocalPosition({ 0,32 });
	scoreTextObject->AddComponent<TextRenderer>("", "tron-arcade.otf", 22, Engine::Color{ 0,0,255,255 });
	auto scoreComponent{ scoreTextObject->AddComponent<ScoreComponent>() };
	scene->Add(std::move(scoreTextObject));

	auto GameloopObject{ std::make_unique<GameObject>() };
	GameloopObject->AddComponent<GameLoop>(mode,scoreComponent);

	scene->Add(std::move(GameloopObject));
}


