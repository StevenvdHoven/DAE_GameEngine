#pragma once
#include "Command.h"

namespace Engine
{
	class MuteSoundCommand : public Command
	{
	public:
		MuteSoundCommand() = default;

		void Execute();
	};

}


