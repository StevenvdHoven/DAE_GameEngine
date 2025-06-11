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

		void PlaySound(const SoundClip& clip) override;

		void PlayMusic(const MusicClip& clip) override;

		SoundClip LoadSound(const std::string& filePath) override;

		MusicClip LoadMusic(const std::string& filePath) override;

		void Mute() override;

		void UnMute() override;

		bool Muted() override;

		void StopAll() override;

	};
}

