#include "SDL_SoundSystem.h"
#include <SDL_mixer.h>
#include <thread>
#include <vector>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <string>
#include <iostream>

using namespace Engine;

class SDL_SoundSystem::SDL_SoundSystem_Pimpl final
{
public:
	SDL_SoundSystem_Pimpl()
	{
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		m_SoundThread = std::jthread{ [this](std::stop_token stopToken)
			{
				SoundLoop(stopToken);
			} 
		};
	}
	~SDL_SoundSystem_Pimpl()
	{
		for (auto& soundClip : m_SoundClips)
		{
			Mix_FreeChunk(soundClip.get());
		}
		m_SoundClips.clear();
		Mix_CloseAudio();
	}

	void SoundLoop(std::stop_token stopToken)
	{
		while (!stopToken.stop_requested())
		{
			std::unique_lock<std::mutex> lock(m_SoundMutex);
			m_SoundCondition.wait(lock, [this] { return m_PlaySounds; });

			while (!m_SoundQueue.empty())
			{
				SoundClip clip = m_SoundQueue.front();
				m_SoundQueue.pop();
				
				if (clip.SoundID >= 0 && clip.SoundID < static_cast<int>(m_SoundClips.size()))
				{
					Mix_PlayChannel(-1, m_SoundClips[clip.SoundID].get(), 0);
				}
			}

			m_PlaySounds = false;
		}
	}

	void Play(const SoundClip& clip)
	{
		if (clip.SoundID < 0 || clip.SoundID >= static_cast<int>(m_SoundClips.size()))
			return;

		std::lock_guard<std::mutex> lock(m_SoundMutex);
		m_SoundQueue.emplace(clip);
	}

	SoundClip LoadSound(const std::string& filepath)
	{
		SoundClip clip;
		if (filepath.find(".wav") == std::string::npos)
		{
			clip.Name = "Invalid";
			clip.SoundID = -1;
			return clip;
		}
		
		Mix_Chunk* mixChunk = Mix_LoadWAV(filepath.c_str());
		if (mixChunk == nullptr)
		{
			clip.Name = "Invalid";
			clip.SoundID = -1;
			return clip;
		}
		m_SoundClips.push_back(std::unique_ptr<Mix_Chunk>(mixChunk));
		clip.Name = filepath;
		clip.SoundID = static_cast<int>(m_SoundClips.size() - 1);
		return clip;
	}

private:
	std::mutex m_SoundMutex;
	std::condition_variable m_SoundCondition;
	bool m_PlaySounds;

	std::vector<std::unique_ptr<Mix_Chunk>> m_SoundClips;
	std::queue<SoundClip> m_SoundQueue;
	std::jthread m_SoundThread;
};

SDL_SoundSystem::SDL_SoundSystem()
{
	m_pimpl = new SDL_SoundSystem_Pimpl();
}

SDL_SoundSystem::~SDL_SoundSystem()
{
	delete m_pimpl;
	m_pimpl = nullptr;
}

void SDL_SoundSystem::Play(const SoundClip& clip)
{
	m_pimpl->Play(clip);
}

SoundClip SDL_SoundSystem::LoadSound(const std::string& filePath)
{
	return m_pimpl->LoadSound(filePath);
}
