#pragma once
#include "GameObject.h"
#include <memory>

class PrefabFactory
{
public:
	static std::unique_ptr<Engine::GameObject> CreatePlayer();
};

