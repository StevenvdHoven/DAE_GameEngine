#pragma once
#include <string>


namespace Engine
{
	struct SoundClip
	{
		std::string Name;
		int SoundID;
	};

	struct MusicClip
	{
		std::string Name;
		int MusicID;
	};

	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;

		virtual void PlaySound(const SoundClip& clip) = 0;
		virtual void PlayMusic(const MusicClip& clip) = 0;
		virtual SoundClip LoadSound(const std::string& filePath) = 0;
		virtual MusicClip LoadMusic(const std::string& filePath) = 0;

	};
}

