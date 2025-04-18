#pragma once
#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <iostream>
#include "TestScene.h"
#include "BattleScene.h"
#include "GameEngine.h"

using namespace Engine;

void load()
{
	//TestScene::CreateScene();
	BattleScene::CreateScene();
}

int main(int, char* []) {

	GameEngine engine("../Data/",480,440);
	engine.Run(load);

	return 0;
}