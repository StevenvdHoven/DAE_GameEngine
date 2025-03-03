#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

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
#include "Scene.h"

using namespace Engine;

void load()
{
	auto scene = SceneManager::GetInstance().CreateScene("Demo");

	auto tempObject{ std::make_unique<GameObject>() };
	auto rawPtr{ tempObject.get() };
	

	auto backgroundObject = std::make_unique<GameObject>();

	backgroundObject->AddComponent<ImageRenderer>("background.tga");
	scene->Add(std::move(backgroundObject));

	auto logoObject = std::make_unique<GameObject>();
	logoObject->AddComponent<ImageRenderer>(std::string{ "logo.tga" });
	auto logoTransform = logoObject->GetTransform();
	logoTransform->SetLocalPosition(216, 180);
	logoTransform->SetLocalRotation(90.f);
	scene->Add(std::move(logoObject));
	scene->Add(std::move(tempObject));

	auto parentObject = std::make_unique<GameObject>();

	auto inputType = ValueCommand<Vector2>::InputType2D::D_PAD;
	InputManager::GetInstance().Bind2DValue(0, new MovePlayerCommand(parentObject.get(), inputType, 100.f));

	parentObject->AddComponent<ImageRenderer>("Red_Ghost.png");
	
	auto parentTransform = parentObject->GetTransform();
	parentTransform->SetLocalPosition(320, 240);

	rawPtr->GetTransform()->SetParent(parentObject.get(), true);

	
	auto childObject = std::make_unique<GameObject>();

	inputType = ValueCommand<Vector2>::InputType2D::WASD;
	InputManager::GetInstance().Bind2DValue(1, new MovePlayerCommand(childObject.get(), inputType, 100.f));
	childObject->AddComponent<ImageRenderer>("Cyan_Ghost.png");
	
	auto childTransform = childObject->GetTransform();
	childTransform->SetLocalPosition(320, 300);

	scene->Add(std::move(parentObject));
	scene->Add(std::move(childObject));

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto textObject = std::make_unique<GameObject>();
	textObject->AddComponent<TextRenderer>("Programming 4 Assignment", font);

	textObject->GetTransform()->SetLocalPosition(80, 20);
	scene->Add(std::move(textObject));
	
	auto fpsCounter = std::make_unique<GameObject>();
	
	fpsCounter->AddComponent<FPSCounterComponent>();
	fpsCounter->AddComponent<TextRenderer>(" ",font);
	
	scene->Add(std::move(fpsCounter));

	rawPtr->GetTransform()->SetParent(nullptr);

	//ViewPort::GetInstance().AddGUI(std::make_unique<Exercise2UI>());
	//ViewPort::GetInstance().AddGUI(std::make_unique<Exercise3::Exercise3UI>());

	scene->Remove(rawPtr);
}

void newLoad()
{
	
}

int main(int, char*[]) {
	GameEngine engine("../Data/");
	engine.Run(load);
    return 0;
}