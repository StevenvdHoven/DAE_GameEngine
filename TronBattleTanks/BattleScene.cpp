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
#include "Scene.h"

using namespace Engine;

void BattleScene::CreateScene()
{
	auto scene{ SceneManager::GetInstance().CreateScene("BattleScene") };

	auto backgroundObject{ std::make_unique<GameObject>() };
	backgroundObject->AddComponent<ImageRenderer>("gameBackground.png");
	scene->Add(std::move(backgroundObject));

	 PrefabFactory::AddPlayer(scene);

	auto Map{ PrefabFactory::Map1Parent(scene) };

	float differenceHeight = 512 - 440;
	scene->MoveScene(Vector2{ 0, differenceHeight });

	// Score 
	auto pFont{ ResourceManager::GetInstance().LoadFont("Lingua.otf",32) };
	auto scoreTextObject{ std::make_unique<GameObject>() };
	scoreTextObject->AddComponent<TextRenderer>("",pFont);
	auto scoreComponent{ scoreTextObject->AddComponent<ScoreComponent>() };
	scene->Add(std::move(scoreTextObject));
	


	auto GameloopObject{ std::make_unique<GameObject>() };
	GameloopObject->AddComponent<GameLoop>(scoreComponent,Map);
	scene->Add(std::move(GameloopObject));
}


