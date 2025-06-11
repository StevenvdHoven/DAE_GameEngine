#include "SkipSceneCommand.h"

SkipSceneCommand::SkipSceneCommand(std::function<void()> func):
	m_Func{func}
{
}

void SkipSceneCommand::Execute()
{
	m_Func();
}
