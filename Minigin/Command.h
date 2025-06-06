#pragma once
#include "GameObject.h"
#include "Vector2.h"

namespace Engine
{
	enum class TriggerState
	{
		PRESSED,
		RELEASED,
		HELD,
		CONSTANT
	};

	enum class DeviceType
	{
		KEYBOARD,
		GAMEPAD
	};

	class Command
	{
	public:
		Command() : m_DeviceType(DeviceType::KEYBOARD), m_TriggerState(TriggerState::PRESSED)
		{

		}
		virtual ~Command() = default;


		virtual void Execute() = 0;

		DeviceType GetDeviceType() const { return m_DeviceType; }
		void ChangeDeviceType(DeviceType deviceType) { m_DeviceType = deviceType; }

		TriggerState GetTriggerState() const { return m_TriggerState; }
		void SetTriggerState(TriggerState triggerState) { m_TriggerState = triggerState; }

	protected:
		DeviceType m_DeviceType;
		TriggerState m_TriggerState;
	};

	template<typename ValueType>
	class ValueCommand
	{
	public:
		enum class InputType
		{
			LEFT_STICK,
			RIGHT_STICK,
			D_PAD,
			WASD,
			ARROW_KEYS,
			RIGHT_TRIGGER,
			LEFT_TRIGGER
		};
		ValueCommand() : m_DeviceType{DeviceType::KEYBOARD},  m_InputType(InputType::LEFT_STICK), m_TriggerState(TriggerState::PRESSED)
		{

		}
		virtual ~ValueCommand() {}

		virtual void Execute(const ValueType& value) = 0;

	public:
		InputType GetInputType() const { return m_InputType; }
		void SetInputType(InputType inputType) { m_InputType = inputType; }

		DeviceType GetDeviceType() const { return m_DeviceType; }
		void ChangeDeviceType(DeviceType deviceType) { m_DeviceType = deviceType; }

		TriggerState GetTriggerState() const { return m_TriggerState; }
		void SetTriggerState(TriggerState triggerState) { m_TriggerState = triggerState; }

	protected:

		DeviceType m_DeviceType;
		InputType m_InputType;
		TriggerState m_TriggerState;
	};

	class GameActorCommand : public Command
	{
		
	public:
		GameActorCommand(GameObject* pActor) : m_pActor(pActor)
		{
		}
		virtual ~GameActorCommand() {}

		

		virtual void Execute() override = 0;
	protected:
		GameObject* GetActor() const { return m_pActor; }
	private:
		GameObject* m_pActor;

	};

	template<typename ValueType>
	class GameActorValueCommand : public ValueCommand<float>
	{
	public:
		GameActorValueCommand(GameObject* pActor, ValueCommand::InputType inputType) :
			m_pActor{ pActor }
		{
			m_DeviceType = DeviceType::KEYBOARD;
			m_InputType = inputType;
			m_TriggerState = TriggerState::HELD;
		}

		virtual void Execute(const float& value) override = 0;
	protected:
		GameObject* GetActor() const { return m_pActor; }
	private:
		GameObject* m_pActor;
	};

	class GameActorCommand2D : public ValueCommand<Vector2>
	{
	
	public:
		GameActorCommand2D(GameObject* pActor, ValueCommand::InputType inputType) : m_pActor(pActor)
		{
			m_DeviceType = DeviceType::KEYBOARD;
			m_InputType = inputType;
			m_TriggerState = TriggerState::HELD;
		}
		virtual ~GameActorCommand2D() {};

		virtual void Execute(const Vector2& value) override = 0;

	protected:
		GameObject* GetActor() const { return m_pActor; }
	private:
		GameObject* m_pActor;
	};
}

