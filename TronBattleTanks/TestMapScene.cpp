#include "TestMapScene.h"
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
#include "Vector2.h"
#include "ServiceLocator.h"
#include <SDL.h>

using namespace Engine;

void TestMapScene::CreateScene()
{
	//ServiceLocator::GetLevelEditor().LoadLevel("Map01.json");
	auto scene = SceneManager::GetInstance().CreateScene("TestMapScene");
	auto pMapObject = PrefabFactory::Map3Parent(scene);
	pMapObject->GetTransform()->SetWorldLocation(Vector2{ 0, 72 });
}
