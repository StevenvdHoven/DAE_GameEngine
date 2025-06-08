#pragma once
#include "Command.h"

namespace Engine
{
	class ToggleLevelEditorCommand : public Command
	{
	public:
		ToggleLevelEditorCommand() = default;
		virtual ~ToggleLevelEditorCommand() = default;

		void Execute() override;
	};

	class CreateGameObjectCommand : public Command
	{
	public:
		CreateGameObjectCommand() = default;

		void Execute() override;
	};
}


