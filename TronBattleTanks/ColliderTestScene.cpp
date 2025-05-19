#include "ColliderTestScene.h"
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

void ColliderTestScene::CreateScene()
{
	auto scene{ SceneManager::GetInstance().CreateScene("Collider Test Scene") };

	auto player{ PrefabFactory::AddPlayer(scene) };
	player->GetTransform()->SetWorldLocation(Vector2{ 200,200 });

	auto pMediumCollider5{ std::make_unique<GameObject>() };
	pMediumCollider5->GetTransform()->SetLocalPosition(64, 278);
	pMediumCollider5->AddComponent<BoxCollider2D>(Vector2{ 48, 17 }, false, LayerMask::Wall);
	scene->Add(std::move(pMediumCollider5));
}
