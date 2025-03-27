#pragma once
#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <iostream>
#include "GameEngine.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "AchievementHandler.h"
#include "Achievements.h"
#include "TextRenderer.h"
#include "ImageRenderer.h"
#include "Transform.h"
#include "FPSCounterComponent.h"
#include "ValueWatchComponent.h"
#include "RotatorComponent.h"
#include "ViewPort.h"
#include <glm.hpp>
#include "Time.h"
#include "Vector2.h"
#include "InputManager.h"
#include "Command.h"
#include "MovePlayerCommand.h"
#include "PlayerDamageCommand.h"
#include "AddScoreCommand.h"
#include "PlayerHealthComponent.h"
#include "HealtBarComponent.h"
#include "ScoreComponent.h"
#include "Observers.h"
#include <SDL.h>
#include "Scene.h"


using namespace Engine;

void load()
{
	auto scene{ SceneManager::GetInstance().CreateScene("Demo") };

	auto backgroundObject{ std::make_unique<GameObject>() };

	backgroundObject->AddComponent<ImageRenderer>("background.tga");
	scene->Add(std::move(backgroundObject));

	auto logoObject{ std::make_unique<GameObject>() };
	logoObject->AddComponent<ImageRenderer>(std::string{ "logo.tga" });
	auto logoTransform = logoObject->GetTransform();
	logoTransform->SetLocalPosition(216, 180);
	logoTransform->SetLocalRotation(90.f);
	scene->Add(std::move(logoObject));

	// Creating player 1
	auto player1{ std::make_unique<GameObject>() };

	auto player1HealthComp{ player1->AddComponent<PlayerHealthComponent>(3) };

	auto inputType{ ValueCommand<Vector2>::InputType2D::D_PAD };
	auto player1MoveCommand{ std::make_unique<MovePlayerCommand>(player1.get(), inputType, 100.f) };
	player1MoveCommand->SetTriggerState(TriggerState::HELD);

	InputManager::GetInstance().Bind2DValue(0, std::move(player1MoveCommand));

	auto player1DamageCommand{ std::make_unique<PlayerDamageCommand>(player1.get()) };
	player1DamageCommand->ChangeDeviceType(Engine::DeviceType::GAMEPAD);
	player1DamageCommand->SetTriggerState(TriggerState::PRESSED);
	InputManager::GetInstance().BindButton(0, 0x4000, std::move(player1DamageCommand));

	player1->AddComponent<ImageRenderer>("Red_Ghost.png");

	auto player1Transform{ player1->GetTransform() };
	player1Transform->SetLocalPosition(320, 240);

	auto healthFont{ ResourceManager::GetInstance().LoadFont("Lingua.otf", 14) };

	auto health1Bar{ std::make_unique<GameObject>() };
	health1Bar->AddComponent<HealtBarComponent>(player1HealthComp);
	health1Bar->AddComponent<TextRenderer>("Health: 3", healthFont);

	auto health1BarTransform{ health1Bar->GetTransform() };
	health1BarTransform->SetParent(player1.get());
	health1BarTransform->SetLocalPosition(0, -20);

	auto score1Text{ std::make_unique<GameObject>() };
	score1Text->GetTransform()->SetLocalPosition(0, 100);
	score1Text->AddComponent<TextRenderer>("Score: 0", healthFont);
	score1Text->AddComponent<ScoreComponent>();

	auto score1Command{ std::make_unique<AddScoreCommand>(score1Text.get()) };
	score1Command->ChangeDeviceType(Engine::DeviceType::GAMEPAD);
	InputManager::GetInstance().BindButton(0, 0x1000, std::move(score1Command));

	AchievementHandler::GetInstance().AddAchievement(std::move(std::make_unique<WinGameAchievement>(1, score1Text.get())));

	scene->Add(std::move(score1Text));

	// Creating player 2
	auto player2{ std::make_unique<GameObject>() };

	auto player2HealthComp{ player2->AddComponent<PlayerHealthComponent>(3) };

	inputType = ValueCommand<Vector2>::InputType2D::WASD;
	auto playerDamageCommand{ std::make_unique<PlayerDamageCommand>(player2.get()) };
	InputManager::GetInstance().Bind2DValue(1, std::move(std::make_unique<MovePlayerCommand>(player2.get(), inputType, 100.f)));
	InputManager::GetInstance().BindButton(1, SDL_SCANCODE_X, std::move(playerDamageCommand));
	player2->AddComponent<ImageRenderer>("Cyan_Ghost.png");

	auto player2Transform{ player2->GetTransform() };
	player2Transform->SetLocalPosition(320, 300);

	auto health2Bar{ std::make_unique<GameObject>() };
	health2Bar->AddComponent<HealtBarComponent>(player2HealthComp);
	health2Bar->AddComponent<TextRenderer>("Health: 3", healthFont);

	auto health2BarTransform{ health2Bar->GetTransform() };
	health2BarTransform->SetParent(player2.get());
	health2BarTransform->SetLocalPosition(0, 0);

	auto score2Text{ std::make_unique<GameObject>() };
	score2Text->GetTransform()->SetLocalPosition(0, 120);
	score2Text->AddComponent<TextRenderer>("Score: 0", healthFont);
	score2Text->AddComponent<ScoreComponent>();

	auto score2Command{ std::make_unique<AddScoreCommand>(score2Text.get()) };
	score2Command->ChangeDeviceType(Engine::DeviceType::KEYBOARD);
	InputManager::GetInstance().BindButton(1, SDL_SCANCODE_Z, std::move(score2Command));

	auto winAchievement{ std::make_unique<WinGameAchievement>(0,score2Text.get()) };
	AchievementHandler::GetInstance().AddAchievement(std::move(winAchievement));

	// Creatine manual
	auto manual1Object{ std::make_unique<GameObject>() };
	manual1Object->AddComponent<TextRenderer>("Player 1: DPAD to move, X to damage yourself, A to add score", healthFont);
	manual1Object->GetTransform()->SetLocalPosition(0, 60);

	auto manual2Object{ std::make_unique<GameObject>() };
	manual2Object->AddComponent<TextRenderer>("Player 2: WASD to move, X to damage yourself, Z to add score", healthFont);
	manual2Object->GetTransform()->SetLocalPosition(0, 75);

	scene->Add(std::move(manual1Object));
	scene->Add(std::move(manual2Object));



	scene->Add(std::move(score2Text));

	scene->Add(std::move(player1));
	scene->Add(std::move(player2));
	scene->Add(std::move(health1Bar));
	scene->Add(std::move(health2Bar));

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto textObject{ std::make_unique<GameObject>() };
	textObject->AddComponent<TextRenderer>("Programming 4 Assignment", font);

	textObject->GetTransform()->SetLocalPosition(80, 20);
	scene->Add(std::move(textObject));

	auto fpsCounter{ std::make_unique<GameObject>() };

	fpsCounter->AddComponent<FPSCounterComponent>();
	fpsCounter->AddComponent<TextRenderer>(" ", font);

	scene->Add(std::move(fpsCounter));

	//ViewPort::GetInstance().AddGUI(std::make_unique<Exercise2UI>());
	//ViewPort::GetInstance().AddGUI(std::make_unique<Exercise3::Exercise3UI>());
}

int main(int, char* []) {

	GameEngine engine("../Data/");
	engine.Run(load);

	return 0;
}