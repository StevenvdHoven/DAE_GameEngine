#pragma once

namespace Engine
{
	class Scene;
}


class BattleScene
{
public:
	static void CreateScene();

private:
	static void AddWorldColliders(Engine::Scene* const pScene);
};

