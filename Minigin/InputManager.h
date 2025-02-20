#pragma once
#include "Singleton.h"

namespace Engine
{

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
	};
}

