#pragma once
#include <memory>
#include "SoundSystem.h"

namespace Engine
{
	class ServiceLocator final
	{
		static std::unique_ptr<SoundSystem> m_SoundSystem;
	public:
		static SoundSystem& GetSoundSystem()
		{
			return *m_SoundSystem;
		}
		static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& soundSystem)
		{
			m_SoundSystem = std::move(soundSystem);
		}
	};
}

