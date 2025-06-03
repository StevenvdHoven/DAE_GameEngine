#pragma once
#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <iostream>
#include "TestScene.h"
#include "BattleScene.h"
#include "MainMenu.h"
#include "MultiPlayerTestScene.h"
#include "ColliderTestScene.h"
#include "GameEngine.h"

using namespace Engine;

void load()
{
	//TestScene::CreateScene();
	//BattleScene::CreateScene();
	MainMenu::CreateScene();
	//ColliderTestScene::CreateScene();
	//MultiPlayerTestScene::CreateScene();
}

int main(int, char* []) {

	GameEngine engine("../Data/",480,512);
	engine.Run(load);

	return 0;
}