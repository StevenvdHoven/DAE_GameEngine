#pragma once
#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "GameEngine.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "ServiceLocator.h"
#include "SDL_SoundSystem.h"
#include "PhysicsSystem.h"
#include "GraphEditor.h"
#include "ViewPort.h"
#include "EngineTime.h"
#include <chrono>
#include <thread>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl2.h>
#include <iostream>

SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

Engine::GameEngine::GameEngine(const std::string& dataPath, int width, int height)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);
	
}

Engine::GameEngine::~GameEngine()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void Engine::GameEngine::Run(const std::function<void()>& load)
{
	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();
	auto& viewPort = ViewPort::GetInstance();
	auto& time = Time::GetInstance();

	ServiceLocator::RegisterSoundSystem(std::move(std::make_unique<SDL_SoundSystem>()));
	ServiceLocator::RegisterPhysicsSystem(std::move(std::make_unique<PhysicsSystem>()));
	ServiceLocator::RegisterGraphEditor(std::move(std::make_unique<GraphEditor>()));

	

	load();

	sceneManager.Start();

	const int ms_per_frame{ 1000 / 144 };
	const float fixed_time_step{ .02f };
	bool doContinue = true;
	auto last_time = std::chrono::high_resolution_clock::now();
	float lag = 0.0f;

	time.m_FixedDeltaTime = fixed_time_step;
	while (doContinue)
	{
		const auto current_time = std::chrono::high_resolution_clock::now();
		const float delta_time = std::chrono::duration<float>(current_time - last_time).count();

		time.m_DeltaTime = delta_time;
		last_time = current_time;
		lag += delta_time;

		doContinue = input.ProcessInput();
		
		while (lag >= fixed_time_step)
		{
			sceneManager.FixedUpdate();
			ServiceLocator::GetPhysicsSystem().FixedUpdate();
			lag -= fixed_time_step;
		}
		
		sceneManager.Update();
		sceneManager.LateUpdate();

		viewPort.Render();
		if(ServiceLocator::GetGraphEditor().IsActive())
			ServiceLocator::GetGraphEditor().GUI();

		renderer.Render();

		const auto sleep_time = current_time + std::chrono::milliseconds(ms_per_frame) - std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(sleep_time);
	}
}
