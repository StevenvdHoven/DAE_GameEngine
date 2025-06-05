#pragma once

namespace Engine
{
	class Scene;
}

enum struct EGameOverType
{
	LOST,
	PLAYER1WON,
	PLAYER2WON,
};


class GameOverScene
{
public:
	static void CreateScene(EGameOverType gameOverType);
};

