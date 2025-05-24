#include "MainMenu.h"
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
#include "MenuComponent.h"
#include <SDL.h>

using namespace Engine;

void MainMenu::CreateScene()
{
	auto scene{ SceneManager::GetInstance().CreateScene("MainMenu") };

	auto backgroundObject{ std::make_unique<GameObject>() };
	backgroundObject->AddComponent<ImageRenderer>("main_menu_bg.png");
	scene->Add(std::move(backgroundObject));

	auto menuObject{ std::make_unique<GameObject>() };
	menuObject->AddComponent<MenuComponent>();
	scene->Add(std::move(menuObject));

}
