#pragma once
#include "Command.h"
#include <functional>

class SkipSceneCommand : public Engine::Command
{
public:
	SkipSceneCommand(std::function<void()> func);

	void Execute();

private:
	std::function<void()> m_Func;
};

