#pragma once
#include "SoundSystem.h"

namespace Engine
{
#define DATAPATH "../Data/"

	class SDL_SoundSystem final : public SoundSystem
	{
		class SDL_SoundSystem_Pimpl;
		SDL_SoundSystem_Pimpl* m_pimpl;
	public:
		SDL_SoundSystem();
		virtual ~SDL_SoundSystem() override;

		// Inherited via SoundSystem
		void PlaySound(const SoundClip& clip) override;

		// Inherited via SoundSystem
		void PlayMusic(const MusicClip& clip) override;

		// Inherited via SoundSystem
		SoundClip LoadSound(const std::string& filePath) override;

		// Inherited via SoundSystem
		MusicClip LoadMusic(const std::string& filePath) override;

	};
}

