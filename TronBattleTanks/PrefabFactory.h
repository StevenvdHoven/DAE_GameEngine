#pragma once
#include "GameObject.h"
#include <string>
#include <vector>
#include <memory>
#include "MenuComponent.h"

namespace Engine
{
	class Scene;
	class Graph;
}

class GameLoop;
enum struct EnemyType;
enum struct EProjectileTarget;

class PrefabFactory
{
public:
	static Engine::GameObject* AddPlayer(Engine::Scene* const scene, int playerIndex = 0,const GameMode mode = GameMode::SinglePlayer);

	static Engine::GameObject* AddPlayerBullet(Engine::Scene* const scene, const EProjectileTarget targetType);

	static Engine::GameObject* AddEnemyBullet(Engine::Scene* const scene);

	static Engine::GameObject* Map1Parent(Engine::Scene* const scene);

	static Engine::GameObject* Map2Parent(Engine::Scene* const scene);

	static Engine::GameObject* Map3Parent(Engine::Scene* const scene);

	static Engine::GameObject* CreateEnemy(Engine::Scene* const scene, EnemyType enemyType,Engine::Graph* pGraph, GameLoop* const gameLoop);

};

