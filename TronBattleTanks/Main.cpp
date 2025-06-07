#pragma once
#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <iostream>
#include "BattleScene.h"
#include "GameOverScene.h"
#include "MainMenu.h"
#include "MultiPlayerTestScene.h"
#include "ColliderTestScene.h"
#include "GameEngine.h"

using namespace Engine;

void load()
{
	//TestScene::CreateScene();
	//BattleScene::CreateScene();
	//MainMenu::CreateScene();
	GameOverScene::CreateScene(EGameOverType::LOST,GameMode::VS,1000);
	//ColliderTestScene::CreateScene();
	//MultiPlayerTestScene::CreateScene();
}

int main(int, char* []) {

	GameEngine engine("../Data/",480,512);
	engine.Run(load);

	return 0;
}