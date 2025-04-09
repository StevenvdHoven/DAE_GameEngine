#include "PrefabFactory.h"
#include "ImageRenderer.h"
#include "PlayerHealthComponent.h"

using namespace Engine;

std::unique_ptr<GameObject> PrefabFactory::CreatePlayer()
{
	auto player{ std::make_unique<GameObject>() };
	player->AddComponent<ImageRenderer>("Red_Ghost.png");
	player->AddComponent<PlayerHealthComponent>(3);

	return player;
}
