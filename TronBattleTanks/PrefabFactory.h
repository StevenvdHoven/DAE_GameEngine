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
	static void AddPlayer(Engine::Scene* const scene);
};

