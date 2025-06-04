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
#include <SDL.h>



using namespace Engine;

#define CENTER_SCREEN Engine::Vector2{480/2,512/2}

void MainMenu::CreateScene()
{
	auto scene{ SceneManager::GetInstance().CreateScene("MainMenu") };

	auto backgroundObject{ std::make_unique<GameObject>() };
	backgroundObject->AddComponent<ImageRenderer>("main_menu_bg.png");
	scene->Add(std::move(backgroundObject));

	auto menuObject{ std::make_unique<GameObject>() };
	menuObject->AddComponent<MenuComponent>();
	scene->Add(std::move(menuObject));

	auto pFont{ ResourceManager::GetInstance().LoadFont("tron-arcade.otf",32) };
	auto singlePlayerText{ std::make_unique<GameObject>() };
	singlePlayerText->GetTransform()->SetLocalPosition(CENTER_SCREEN + Vector2{ -200,-50 });
	singlePlayerText->AddComponent<TextRenderer>("Single Player",pFont,Engine::Color{0,0,255,255});
	scene->Add(std::move(singlePlayerText));

	auto CoOpText{ std::make_unique<GameObject>() };
	CoOpText->GetTransform()->SetLocalPosition(CENTER_SCREEN + Vector2{ -200,50 });
	CoOpText->AddComponent<TextRenderer>("Co Op", pFont, Engine::Color{ 0,0,255,255 });
	scene->Add(std::move(CoOpText));

	auto pvpText{ std::make_unique<GameObject>() };
	pvpText->GetTransform()->SetLocalPosition(CENTER_SCREEN + Vector2{ -25,175 });
	pvpText->AddComponent<TextRenderer>("PVP", pFont, Engine::Color{ 0,0,255,255 });
	scene->Add(std::move(pvpText));
	
}
