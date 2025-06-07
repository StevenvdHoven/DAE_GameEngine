#pragma once
#include "Command.h"

namespace Engine
{
	class ToggleLevelEditorCommand : Engine::Command
	{
	public:
		ToggleLevelEditorCommand() = default;

		void Execute() override;
	};

	class CreateGameObjectCommand : Engine::Command
	{
	public:
		CreateGameObjectCommand() = default;

		void Execute() override;
	};
}


