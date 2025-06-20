#pragma once
#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <iostream>
#include "BattleScene.h"
#include "TestMapScene.h"
#include "GameOverScene.h"
#include "MainMenu.h"
#include "MultiPlayerTestScene.h"
#include "ColliderTestScene.h"
#include "ServiceLocator.h"
#include "SimpleTriggerComponent.h"
#include "AutoDestroyComponent.h"
#include "Scoreboard.h"
#include "GameEngine.h"

using namespace Engine;

void load()
{
	ServiceLocator::GetComponentRegistery().Register<SimpleTriggerComponent>("SimpleTriggerComponent");
	ServiceLocator::GetComponentRegistery().Register<Scoreboard>("Scoreboard");
	ServiceLocator::GetComponentRegistery().Register<AutoDestroyComponent>("AutoDestroyComponent");
	//TestScene::CreateScene();
	//BattleScene::CreateScene);
	MainMenu::CreateScene();
	//GameOverScene::CreateScene(EGameOverType::LOST,GameMode::SinglePlayer,1000);
	//TestMapScene::CreateScene();
	//ColliderTestScene::CreateScene();
	//MultiPlayerTestScene::CreateScene();
}

int main(int, char* []) {

	GameEngine engine("../Data/",480,512);
	engine.Run(load);

	return 0;
}