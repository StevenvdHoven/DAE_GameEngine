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
		Command() : m_DeviceType(DeviceType::KEYBOARD)
		{

		}
		virtual ~Command() = default;

		Command(const Command& other)
		{
			m_DeviceType = other.m_DeviceType;
		}

		Command(Command&& other) noexcept
		{
			m_DeviceType = other.m_DeviceType;
		}

		Command& operator=(const Command& other)
		{
			if (this != &other)
			{
				m_DeviceType = other.m_DeviceType;
			}
			return *this;
		}

		Command& operator=(Command&& other) noexcept
		{
			if (this != &other)
			{
				m_DeviceType = other.m_DeviceType;
			}
			return *this;
		}


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
		ValueCommand() : m_InputType(InputType2D::LEFT_STICK)
		{

		}
		virtual ~ValueCommand() {}

		ValueCommand(const ValueCommand& other)
		{
			m_InputType = other.m_InputType;
		}

		ValueCommand(ValueCommand&& other) noexcept
		{
			m_InputType = other.m_InputType;
		}

		ValueCommand& operator=(const ValueCommand& other)
		{
			if (this != &other)
			{
				m_InputType = other.m_InputType;
			}
			return *this;
		}

		ValueCommand& operator=(ValueCommand&& other) noexcept
		{
			if (this != &other)
			{
				m_InputType = other.m_InputType;
			}
			return *this;
		}

		virtual void Execute(const ValueType& value) = 0;

	public:
		InputType2D GetInputType() const { return m_InputType; }

	protected:
		InputType2D m_InputType;
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

	class GameActorCommand2D : public ValueCommand<Vector2>
	{
	
	public:
		GameActorCommand2D(GameObject* pActor, ValueCommand::InputType2D inputType) : m_pActor(pActor)
		{
			m_InputType = inputType;
		}
		virtual ~GameActorCommand2D() {};

		virtual void Execute(const Vector2& value) override = 0;

	protected:
		GameObject* GetActor() const { return m_pActor; }
	private:
		GameObject* m_pActor;
	};
}

