#pragma once
#include "GameObject.h"
#include <string>
#include <vector>
#include <memory>

namespace Engine
{
	class Scene;
}

class GameLoop;
enum struct EnemyType;

class PrefabFactory
{
public:
	static Engine::GameObject* AddPlayer(Engine::Scene* const scene, int playerIndex = 0);

	static Engine::GameObject* AddPlayerBullet(Engine::Scene* const scene);

	static Engine::GameObject* AddEnemyBullet(Engine::Scene* const scene);

	static Engine::GameObject* Map1Parent(Engine::Scene* const scene);

	static Engine::GameObject* Map2Parent(Engine::Scene* const scene);

	static Engine::GameObject* Map3Parent(Engine::Scene* const scene);

	static Engine::GameObject* CreateEnemy(Engine::Scene* const scene, EnemyType enemyType, GameLoop* const gameLoop);

};

