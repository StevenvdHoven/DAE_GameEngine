#pragma once
#include "GameObject.h"
#include <memory>

namespace Engine
{
	class Scene;
}

class GameLoop;

class PrefabFactory
{
public:
	static Engine::GameObject* AddPlayer(Engine::Scene* const scene, int playerIndex = 0);

	static Engine::GameObject* AddPlayerBullet(Engine::Scene* const scene);

	static Engine::GameObject* AddEnemyBullet(Engine::Scene* const scene);

	static Engine::GameObject* Map1Parent(Engine::Scene* const scene);

	static Engine::GameObject* CreateEnemy(Engine::Scene* const scene, GameLoop* const gameLoop);

};

