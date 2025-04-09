#include "SDL_SoundSystem.h"
#include <SDL_mixer.h>
#include <thread>
#include <vector>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <string>
#include <iostream>

using namespace Engine;

class SDL_SoundSystem::SDL_SoundSystem_Pimpl final
{
public:
	SDL_SoundSystem_Pimpl():
		m_PlaySounds{false},
		m_SoundClips{},
		m_MusicClips{}
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

		m_SoundThread.request_stop();
		m_SoundCondition.notify_all();
		m_SoundThread.join();

		Mix_HaltChannel(-1);
		Mix_HaltMusic();

		for (auto& sound : m_SoundClips)
		{
			Mix_FreeChunk(sound.second);
		}
		m_SoundClips.clear();

		for (auto& music : m_MusicClips)
		{
			Mix_FreeMusic(music.second);
		}
		m_MusicClips.clear();

		Mix_CloseAudio();
		std::cout << "m_MusicClips cleared\n";
	}

	void SoundLoop(std::stop_token stopToken)
	{
		while (!stopToken.stop_requested())
		{
			std::unique_lock<std::mutex> lock(m_SoundMutex);
			m_SoundCondition.wait(lock, [this,&stopToken] { return m_PlaySounds || stopToken.stop_requested(); });

			if (stopToken.stop_requested()) break;

			while (!m_SoundQueue.empty())
			{
				SoundClip clip = m_SoundQueue.front();
				m_SoundQueue.pop();
				
				if (clip.SoundID >= 0 && clip.SoundID < static_cast<int>(m_SoundClips.size()))
				{
					Mix_PlayChannel(-1, m_SoundClips[clip.Name], 0);
				}
			}

			while(!m_MusicQueue.empty())
			{
				MusicClip clip = m_MusicQueue.front();
				m_MusicQueue.pop();

				if (clip.MusicID >= 0 && clip.MusicID < static_cast<int>(m_MusicClips.size()))
				{
					Mix_PlayMusic(m_MusicClips[clip.Name], -1);
				}
			}

			m_PlaySounds = false;
		}

		printf("Sound thread stopped\n");
	}

	void PlaySound(const SoundClip& clip)
	{
		if (clip.SoundID < 0 || clip.SoundID >= static_cast<int>(m_SoundClips.size()))
			return;

		std::lock_guard<std::mutex> lock(m_SoundMutex);
		m_SoundQueue.emplace(clip);

		m_PlaySounds = true;
		m_SoundCondition.notify_all();
	}

	void PlayMusic(const MusicClip& clip)
	{
		if (clip.MusicID < 0 || clip.MusicID >= static_cast<int>(m_MusicClips.size()))
			return;

		std::lock_guard<std::mutex> lock(m_SoundMutex);
		m_MusicQueue.emplace(clip);

		m_PlaySounds = true;
		m_SoundCondition.notify_all();
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

		if (m_SoundClips.find(filepath) != m_SoundClips.end())
		{
			clip.Name = filepath;
			clip.SoundID = static_cast<int>(m_SoundClips.size() - 1);
			return clip;
		}

		auto fullpath = DATAPATH + filepath;
		
		Mix_Chunk* mixChunk = Mix_LoadWAV(fullpath.c_str());
		if (mixChunk == nullptr)
		{
			clip.Name = "Invalid";
			clip.SoundID = -1;

			return clip;
		}


		m_SoundClips.emplace(filepath,mixChunk);
		
		clip.Name = filepath;
		clip.SoundID = static_cast<int>(m_SoundClips.size() - 1);
		return clip;
	}

	MusicClip LoadMusic(const std::string& filepath)
	{
		MusicClip clip;
		if (filepath.find(".wav") == std::string::npos)
		{
			clip.Name = "Invalid";
			clip.MusicID = -1;
			return clip;
		}

		if (m_MusicClips.find(filepath) != m_MusicClips.end())
		{
			clip.Name = filepath;
			clip.MusicID = static_cast<int>(m_MusicClips.size() - 1);
			return clip;
		}

		auto fullpath = DATAPATH + filepath;

		Mix_Music* mixMusic = Mix_LoadMUS(fullpath.c_str());
		if (mixMusic == nullptr)
		{
			clip.Name = "Invalid";
			clip.MusicID = -1;
			return clip;
		}
		m_MusicClips.emplace(filepath,mixMusic);

		clip.Name = filepath;
		clip.MusicID = static_cast<int>(m_MusicClips.size() - 1);
		return clip;
	}

private:
	std::mutex m_SoundMutex;
	std::condition_variable m_SoundCondition;
	bool m_PlaySounds;

	std::unordered_map<std::string,Mix_Chunk*> m_SoundClips;
	std::unordered_map<std::string,Mix_Music*> m_MusicClips;
	std::queue<SoundClip> m_SoundQueue;
	std::queue<MusicClip> m_MusicQueue;
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

void SDL_SoundSystem::PlaySound(const SoundClip& clip)
{
	m_pimpl->PlaySound(clip);
}

void Engine::SDL_SoundSystem::PlayMusic(const MusicClip& clip)
{
	m_pimpl->PlayMusic(clip);
}

SoundClip SDL_SoundSystem::LoadSound(const std::string& filePath)
{
	return m_pimpl->LoadSound(filePath);
}

MusicClip Engine::SDL_SoundSystem::LoadMusic(const std::string& filePath)
{
	return m_pimpl->LoadMusic(filePath);
}


