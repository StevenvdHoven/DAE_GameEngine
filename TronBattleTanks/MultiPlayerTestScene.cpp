#include "MultiPlayerTestScene.h"
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

void MultiPlayerTestScene::CreateScene()
{
	auto scene{ SceneManager::GetInstance().CreateScene("MultiPlayerInputTest") };

	auto pPlayer1{ PrefabFactory::AddPlayer(scene) };
	pPlayer1->GetTransform()->SetLocalPosition(200, 200);

	auto pPlayer2{ PrefabFactory::AddPlayer(scene,1) };
	pPlayer2->GetTransform()->SetLocalPosition(300, 200);
}
