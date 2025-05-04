#include "PrefabFactory.h"
#include "ImageRenderer.h"
#include "PlayerHealthComponent.h"
#include "BoxCollider2D.h"
#include "PhysicsBody.h"
#include "Vector2.h"

#include "InputManager.h"
#include "MovePlayerCommand.h"

using namespace Engine;

std::unique_ptr<GameObject> PrefabFactory::CreatePlayer()
{
	auto player{ std::make_unique<GameObject>() };
	player->AddComponent<ImageRenderer>("Red_Ghost.png");
	player->AddComponent<PlayerHealthComponent>(3);
	auto collider{ player->AddComponent<BoxCollider2D>(Vector2{ 28, 28 }, false) };
	collider->Center = Vector2{ 2.f, 2.f };
	player->AddComponent<PhysicsBody>();

	auto moveCommand = std::make_unique<MovePlayerCommand>(player.get(), Engine::ValueCommand<Vector2>::InputType2D::ARROW_KEYS,1000.f);
	InputManager::GetInstance().Bind2DValue(0, std::move(moveCommand));

	return player;
}
