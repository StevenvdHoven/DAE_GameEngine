#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#pragma once
#include <iostream>
#include "GameEngine.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextRenderer.h"
#include "ImageRenderer.h"
#include "Transform.h"
#include "FPSCounterComponent.h"
#include "ValueWatchComponent.h"
#include "RotatorComponent.h"
#include "Exercise2UI.h"
#include "Exercise3UI.h"
#include "ViewPort.h"
#include <glm.hpp>
#include "Time.h"
#include "Vector2.h"
#include "InputManager.h"
#include "Command.h"
#include "MovePlayerCommand.h"
#include "PlayerDamageCommand.h"
#include "PlayerHealthComponent.h"
#include "HealtBarComponent.h"
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
	
	InputManager::GetInstance().Bind2DValue(0, std::move(std::make_unique<MovePlayerCommand>(player1.get(), inputType, 100.f)));

	auto player1DamageCommand{ std::make_unique<PlayerDamageCommand>(player1.get()) };
	player1DamageCommand->ChangeDeviceType(Engine::DeviceType::GAMEPAD);
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
	health1BarTransform->SetLocalPosition(0,-20);

	// Creating player 2
	auto player2{ std::make_unique<GameObject>() };

	auto player2HealthComp{ player2->AddComponent<PlayerHealthComponent>(3) };

	inputType = ValueCommand<Vector2>::InputType2D::WASD;
	auto playerDamageCommand{ std::make_unique<PlayerDamageCommand>(player2.get()) };
	InputManager::GetInstance().Bind2DValue(1,std::move(std::make_unique<MovePlayerCommand>(player2.get(), inputType, 100.f)));
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
	fpsCounter->AddComponent<TextRenderer>(" ",font);
	
	scene->Add(std::move(fpsCounter));

	//ViewPort::GetInstance().AddGUI(std::make_unique<Exercise2UI>());
	//ViewPort::GetInstance().AddGUI(std::make_unique<Exercise3::Exercise3UI>());
}

void newLoad()
{
	
}

int main(int, char*[]) {
	GameEngine engine("../Data/");
	engine.Run(load);
    return 0;
}