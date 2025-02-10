#pragma once
#include "Singleton.h"


class InputManager final : public Singleton<InputManager>
{
public:
	bool ProcessInput();
};


