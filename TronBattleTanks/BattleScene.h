#pragma once
class Scene;

class BattleScene
{
public:
	static void CreateScene();

private:
	static void AddWorldColliders(Scene* const pScene);
};

