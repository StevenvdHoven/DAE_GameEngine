#pragma once
#include <string>


namespace Engine
{
	struct SoundClip
	{
		std::string Name;
		int SoundID;
		float durration;
	};

	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;

		virtual void Play(const SoundClip& clip) = 0;
		virtual SoundClip LoadSound(const std::string& filePath) = 0;

	};
}

