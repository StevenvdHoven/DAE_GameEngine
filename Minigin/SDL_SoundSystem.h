#pragma once
#include "SoundSystem.h"

namespace Engine
{
	class SDL_SoundSystem final : public SoundSystem
	{
		class SDL_SoundSystem_Pimpl;
		SDL_SoundSystem_Pimpl* m_pimpl;
	public:
		SDL_SoundSystem();
		virtual ~SDL_SoundSystem() override;
		// Inherited via SoundSystem
		void Play(const SoundClip& clip) override;

		// Inherited via SoundSystem
		SoundClip LoadSound(const std::string& filePath) override;
	};
}

