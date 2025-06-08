#include "GameOverScene.h"
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
#include "GameOverMenuComponent.h"
#include "ServiceLocator.h"
#include "TextRenderer.h"
#include "Scene.h"
#include <SDL.h>

using namespace Engine;

void GameOverScene::CreateScene(EGameOverType type, GameMode mode, int score)
{
	auto scene{ SceneManager::GetInstance().CreateScene("GameOver Scene") };

	auto gameOverObject{ std::make_unique<GameObject>() };
	gameOverObject->AddComponent<GameOverMenuComponent>(mode);

	std::string text{ "Game Over" };

	if (type != EGameOverType::LOST)
	{
		text = type == EGameOverType::PLAYER1WON ? "Player 1 Won!" : "Player 2 Won!";
	}

	auto gameOverText{ std::make_unique<GameObject>() };
	gameOverText->GetTransform()->SetLocalPosition({ 50, 50 });
	gameOverText->AddComponent<TextRenderer>(text, "tron-arcade.otf", 30, Engine::Color{ 0,0,255,255 });

	if (mode != GameMode::VS)
	{
		auto ScoreText{ std::make_unique<GameObject>() };
		ScoreText->GetTransform()->SetLocalPosition({ 50,200 });
		ScoreText->AddComponent<TextRenderer>("Score " + std::to_string(score), "tron-arcade.otf", 30, Engine::Color{ 0,0,255,255 });

		scene->Add(std::move(ScoreText));
	}
	
	scene->Add(std::move(gameOverText));
	scene->Add(std::move(gameOverObject));

	
	
}
