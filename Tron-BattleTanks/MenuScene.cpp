#include "MenuScene.h"
#include "../Minigin/GameObject.h"
#include "../Minigin/ImageRenderer.h"
#include "../Minigin/Transform.h"

std::unique_ptr<Scene> MenuScene::CreateMenuScene()
{
	auto scene = std::make_unique<Scene>("Menu");

	auto backgroundObject = std::make_unique<GameObject>();
	backgroundObject->AddComponent<ImageRenderer>("background.tga");
	scene->Add(std::move(backgroundObject));

	auto logoObject = std::make_unique<GameObject>();
	logoObject->AddComponent<ImageRenderer>("logo.tga");
	auto logoTransform = logoObject->GetTransform();
	logoTransform->SetLocalPosition(216, 180);


	return std::move(scene);
}
