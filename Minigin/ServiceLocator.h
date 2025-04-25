#pragma once
#include <memory>
#include "SoundSystem.h"
#include "PhysicsSystem.h"

namespace Engine
{
	class ServiceLocator final
	{
		static std::unique_ptr<SoundSystem> m_SoundSystem;
		static std::unique_ptr<PhysicsSystem> m_PhysicsSystem;
	public:

		static SoundSystem& GetSoundSystem()
		{
			return *m_SoundSystem;
		}
		static PhysicsSystem& GetPhysicsSystem()
		{
			return *m_PhysicsSystem;
		}

		static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& soundSystem)
		{
			m_SoundSystem = std::move(soundSystem);
		}
		static void RegisterPhysicsSystem(std::unique_ptr<PhysicsSystem>&& physicsSystem)
		{
			m_PhysicsSystem = std::move(physicsSystem);
		}
	};
}

