#pragma once
#include "../Minigin/Scene.h"
#include <xmemory>

class MenuScene final
{
public:
	static std::unique_ptr<Scene> CreateMenuScene();
};

