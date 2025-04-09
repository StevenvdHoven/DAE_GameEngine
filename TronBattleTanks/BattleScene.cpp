#include "BattleScene.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "ImageRenderer.h"
#include "PrefabFactory.h"
#include "Transform.h"

using namespace Engine;

void BattleScene::CreateScene()
{
	auto scene{ SceneManager::GetInstance().CreateScene("BattleScene") };

	auto backgroundObject{ std::make_unique<GameObject>() };
	backgroundObject->AddComponent<ImageRenderer>("gameBackground.png");
	scene->Add(std::move(backgroundObject));

	auto player1{ PrefabFactory::CreatePlayer() };
	auto player1Transform{ player1->GetTransform() };
	player1Transform->SetLocalPosition(100, 100);

	scene->Add(std::move(player1));
}
