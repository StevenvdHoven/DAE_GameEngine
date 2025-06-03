#pragma once

namespace Engine
{
	class Scene;
}

enum struct GameMode;

class BattleScene
{
public:
	static void CreateScene(GameMode mode);
};

