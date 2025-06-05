#pragma once
#include <memory.h>
#include "Singleton.h"
#include "Command.h"
#include "Vector2.h"

namespace Engine
{
	class PlayerBindingButton
	{
	public:
		int InputMask;
		bool Pressed;
		Command* pCommand;
	};

	class PlayerBindingValue
	{
	public:
		int InputMask;
		bool Pressed;
		ValueCommand<float>* pCommand;
	};

	class PlayerBinding2DValue
	{
	public:
		bool Pressed;
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

		void Unbind(int playerIndex, void* pCommand);
	};
}

