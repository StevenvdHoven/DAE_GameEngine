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
#include "TextRenderer.h"
#include "StartGameCommand.h"
#include "InputManager.h"
#include "Scene.h"
#include "MenuComponent.h"
#include "ServiceLocator.h"
#include <SDL.h>



using namespace Engine;

#define CENTER_SCREEN Engine::Vector2{480/2,512/2}

void MainMenu::CreateScene()
{
	auto scene{ ServiceLocator::GetLevelEditor().LoadLevel("MainMenu.json")};

	auto menuObject{ std::make_unique<GameObject>() };
	menuObject->AddComponent<MenuComponent>();
	scene->Add(std::move(menuObject));
	
}
