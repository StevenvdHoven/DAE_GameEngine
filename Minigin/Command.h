#pragma once
#include "GameObject.h"
#include "Vector2.h"

namespace Engine
{
	enum class DeviceType
	{
		KEYBOARD,
		GAMEPAD
	};

	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;

		DeviceType GetDeviceType() const { return m_DeviceType; }
	protected:
		DeviceType m_DeviceType;
	};

	template<typename ValueType>
	class ValueCommand
	{
	public:
		enum class InputType2D
		{
			LEFT_STICK,
			RIGHT_STICK,
			D_PAD,
			WASD,
			ARROW_KEYS
		};

		virtual ~ValueCommand() = default;
		virtual void Execute(const ValueType& value) = 0;
	public:
		InputType2D GetInputType() const { return m_InputType; }
	protected:
		InputType2D m_InputType;
	};

	class GameActorCommand : public Command
	{
		GameObject* m_pActor;
	protected:
		GameObject* GetActor() const { return m_pActor; }
	public:
		GameActorCommand(GameObject* pActor) : m_pActor(pActor) {}
		virtual void Execute() override = 0;

	};

	class GameActorCommand2D : public ValueCommand<Vector2>
	{
	protected:
		GameObject* GetActor() const { return m_pActor; }
	public:
		GameActorCommand2D(GameObject* pActor, ValueCommand::InputType2D inputType) : m_pActor(pActor) 
		{
			m_InputType = inputType;
		}
		virtual void Execute(const Vector2& value) override = 0;
	private:
		GameObject* m_pActor;
	};
}

