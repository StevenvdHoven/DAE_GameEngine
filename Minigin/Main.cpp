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
#include <glm.hpp>
#include "Time.h"
#include "Vector2.h"
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
	parentObject->AddComponent<ImageRenderer>("Red_Ghost.png");
	parentObject->AddComponent<RotatorComponent>(Vector2{ 320,240 },false, 100.f, 0.f, 5.f);

	auto parentTransform = parentObject->GetTransform();
	parentTransform->SetLocalPosition(320, 240);

	rawPtr->GetTransform()->SetParent(parentObject.get(), true);

	auto childObject = std::make_unique<GameObject>();
	childObject->AddComponent<ImageRenderer>("Cyan_Ghost.png");
	childObject->AddComponent<RotatorComponent>(Vector2{ 0,0 },true, 100.f, 0.f, 5.f);

	auto childTransform = childObject->GetTransform();
	childTransform->SetParent(parentObject.get(), true);
	childTransform->SetLocalPosition(50, 0);

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

	scene->Remove(rawPtr);
}

int main(int, char*[]) {
	GameEngine engine("../Data/");
	engine.Run(load);
    return 0;
}