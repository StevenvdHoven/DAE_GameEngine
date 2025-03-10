#pragma once
#include <memory.h>
#include "Singleton.h"
#include "Command.h"
#include "Vector2.h"

namespace Engine
{
	struct PlayerBindingButton
	{
		int InputMask;
		Command* pCommand;

	};

	struct PlayerBindingValue
	{
		int InputMask;
		ValueCommand<float>* pCommand;
	};

	struct PlayerBinding2DValue
	{
		ValueCommand<Vector2>* pCommand; 
	};

	class InputManager final : public Singleton<InputManager>
	{
		class InputManagerImpl;
		InputManagerImpl* m_pImpl;

		bool ProcessKeyboard();

	public:
		InputManager();
		~InputManager();
		bool ProcessInput();

		void BindButton(int playerIndex, int button, std::unique_ptr<Command> pCommand);
		void BindValue(int playerIndex, int button, std::unique_ptr <ValueCommand<float>> pCommand);
		void Bind2DValue(int playerIndex, std::unique_ptr <ValueCommand<Vector2>> pCommand);
	};
}

