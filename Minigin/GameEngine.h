#pragma once
#include <string>
#include <functional>

namespace Engine
{
	class GameEngine
	{
	public:
		explicit GameEngine(const std::string& dataPath, int width, int height);
		~GameEngine();
		void Run(const std::function<void()>& load);

		GameEngine(const GameEngine& other) = delete;
		GameEngine(GameEngine&& other) = delete;
		GameEngine& operator=(const GameEngine& other) = delete;
		GameEngine& operator=(GameEngine&& other) = delete;
	};
}
