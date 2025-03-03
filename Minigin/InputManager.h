#pragma once
#include "Singleton.h"

namespace Engine
{

	class InputManager final : public Singleton<InputManager>
	{
		class InputManagerImpl;
		InputManagerImpl* m_pImpl;

		bool ProcessKeyboard();

	public:
		bool ProcessInput();
		bool Is_Pressed(unsigned int button) const;
		bool Is_Released(unsigned int button) const;
		bool Is_Down(unsigned int button) const;
	};
}

