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
#include "Scene.h"
#include <SDL.h>

using namespace Engine;

void GameOverScene::CreateScene(EGameOverType)
{
	auto scene{ SceneManager::GetInstance().CreateScene("GameOver Scene") };

	auto gameOverObjext{ std::make_unique<GameObject>() };
	auto pFont{ ResourceManager::GetInstance().LoadFont("tron-arcade.otf",22) };
	gameOverObjext->AddComponent<TextRenderer>("Game over press START to go to menu", pFont);

	scene->Add(std::move(gameOverObjext));
}
