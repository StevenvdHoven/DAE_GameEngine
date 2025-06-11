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
#include <SDL.h>
#include "InputManager.h"
#include "SoundSystemCommands.h"

using namespace Engine;

/// <summary>
/// Custom deleter for mix chunk.
/// NOTE: It caused a crash since Mix_FreeChunk already deletes what was in there so a custom deleter or wrapper class was a good solution.			
/// </summary>
struct Custom_MixChunk_Deleter
{
	Custom_MixChunk_Deleter() {};
	Custom_MixChunk_Deleter(const Custom_MixChunk_Deleter&) {};
	Custom_MixChunk_Deleter(Custom_MixChunk_Deleter&) {};
	Custom_MixChunk_Deleter(Custom_MixChunk_Deleter&&) noexcept {};

	void operator()(Mix_Chunk* chunk)
	{
		Mix_FreeChunk(chunk);
	};
};

/// <summary>
/// Custom deleter for mix music.
/// NOTE: It caused a crash since Mix_FreeMusic already deletes what was in there so a custom deleter or wrapper class was a good solution.
/// </summary>
struct Custom_MixMusic_Deleter
{
	Custom_MixMusic_Deleter() {};
	Custom_MixMusic_Deleter(const Custom_MixMusic_Deleter&) {};
	Custom_MixMusic_Deleter(Custom_MixMusic_Deleter&) {};
	Custom_MixMusic_Deleter(Custom_MixMusic_Deleter&&) noexcept {};
	void operator()(Mix_Music* music)
	{
		Mix_FreeMusic(music);
	};
};

class SDL_SoundSystem::SDL_SoundSystem_Pimpl final
{
public:
	SDL_SoundSystem_Pimpl():
		m_PlaySounds{false},
		m_Muted{false},
		m_SoundClips{},
		m_MusicClips{}
	{
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		m_SoundThread = std::jthread{ [this](std::stop_token stopToken)
			{
				SoundLoop(stopToken);
			} 
		};

		auto muteAudioCommand{ std::make_unique<MuteSoundCommand>() };
		muteAudioCommand->ChangeDeviceType(DeviceType::KEYBOARD);
		muteAudioCommand->SetTriggerState(TriggerState::PRESSED);
		InputManager::GetInstance().BindButton(0, SDL_SCANCODE_F2, std::move(muteAudioCommand));
	}
	~SDL_SoundSystem_Pimpl()
	{

		m_SoundThread.request_stop();
		m_SoundCondition.notify_all();
		m_SoundThread.join();

		Mix_HaltChannel(-1);
		Mix_HaltMusic();

		Mix_CloseAudio();
		std::cout << "m_MusicClips cleared\n";
	}

	void SoundLoop(std::stop_token stopToken)
	{
		while (!stopToken.stop_requested())
		{
			std::unique_lock<std::mutex> lock(m_LoopMutex);
			m_SoundCondition.wait(lock, [this,&stopToken] { return m_PlaySounds || stopToken.stop_requested(); });

			if (stopToken.stop_requested()) break;

			lock.unlock();

			while (!m_SoundQueue.empty())
			{
				std::lock_guard<std::mutex> soundLock(m_SoundMutex);

				SoundClip clip = m_SoundQueue.front();
				m_SoundQueue.pop();


				if (clip.SoundID >= 0 && clip.SoundID < static_cast<int>(m_SoundClips.size()))
				{
					Mix_PlayChannel(-1, m_SoundClips[clip.Name].get(), 0);
				}
			}

			while(!m_MusicQueue.empty())
			{
				std::lock_guard<std::mutex> musicLock(m_MusicMutex);

				MusicClip clip = m_MusicQueue.front();
				m_MusicQueue.pop();


				if (clip.MusicID >= 0 && clip.MusicID < static_cast<int>(m_MusicClips.size()))
				{
					Mix_PlayMusic(m_MusicClips[clip.Name].get(), -1);
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

		std::lock_guard<std::mutex> lock(m_MusicMutex);
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

		Custom_MixChunk_Deleter d{};
		auto pChunck = std::unique_ptr<Mix_Chunk, Custom_MixChunk_Deleter>(mixChunk,d);

		std::lock_guard<std::mutex> lock(m_SoundMutex);
		m_SoundClips.emplace(filepath, std::move(pChunck));
		
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

		Custom_MixMusic_Deleter d{};
		auto pMusic = std::unique_ptr<Mix_Music, Custom_MixMusic_Deleter>(mixMusic, d);

		std::lock_guard<std::mutex> lock(m_MusicMutex);
		m_MusicClips.emplace(filepath,std::move(pMusic));

		clip.Name = filepath;
		clip.MusicID = static_cast<int>(m_MusicClips.size() - 1);
		return clip;
	}

	void Mute()
	{
		Mix_Volume(-1, 0);
		Mix_VolumeMusic(0);
		m_Muted = true;
	}

	void UnMute()
	{
		Mix_Volume(-1, MIX_MAX_VOLUME);
		Mix_VolumeMusic(MIX_MAX_VOLUME);
		m_Muted = false;
	}

	bool Muted() 
	{
		return m_Muted;
	}

private:
	std::mutex m_LoopMutex;
	std::mutex m_SoundMutex;
	std::mutex m_MusicMutex;
	std::condition_variable m_SoundCondition;
	bool m_PlaySounds;
	bool m_Muted;

	std::unordered_map<std::string, std::unique_ptr<Mix_Chunk, Custom_MixChunk_Deleter>> m_SoundClips;
	std::unordered_map<std::string,std::unique_ptr<Mix_Music,Custom_MixMusic_Deleter>> m_MusicClips;
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

void Engine::SDL_SoundSystem::Mute()
{
	m_pimpl->Mute();
}

void Engine::SDL_SoundSystem::UnMute()
{
	m_pimpl->UnMute();
}

bool Engine::SDL_SoundSystem::Muted()
{
	return m_pimpl->Muted();
}


