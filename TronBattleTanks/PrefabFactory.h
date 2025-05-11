#pragma once
#include "GameObject.h"
#include <memory>

namespace Engine
{
	class Scene;
}

class PrefabFactory
{
public:
	static Engine::GameObject* AddPlayer(Engine::Scene* const scene);

	static Engine::GameObject* AddPlayerBullet(Engine::Scene* const scene);
};

