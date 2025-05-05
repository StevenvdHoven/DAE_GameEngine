#include "PrefabFactory.h"
#include "ImageRenderer.h"
#include "PlayerHealthComponent.h"
#include "BoxCollider2D.h"
#include "PhysicsBody.h"
#include "Vector2.h"
#include "Scene.h"

#include "InputManager.h"
#include "MovePlayerCommand.h"

using namespace Engine;

void PrefabFactory::AddPlayer(Scene* const scene)
{
	auto player{ std::make_unique<GameObject>() };
	player->GetTransform()->SetLocalPosition(100, 100);
	player->AddComponent<ImageRenderer>("player_body.png");
	player->AddComponent<PlayerHealthComponent>(3);
	auto collider{ player->AddComponent<BoxCollider2D>(Vector2{ 28, 28 }, false) };
	collider->Center = Vector2{ 2.f, 2.f };
	player->AddComponent<PhysicsBody>();


	// Bind Input
	auto moveCommand = std::make_unique<MovePlayerCommand>(player.get(), Engine::ValueCommand<Vector2>::InputType2D::ARROW_KEYS,1000.f);
	moveCommand->SetTriggerState(TriggerState::HELD);

	auto moveCommandController = std::make_unique<MovePlayerCommand>(player.get(), Engine::ValueCommand<Vector2>::InputType2D::D_PAD, 1000.f);
	moveCommandController->SetTriggerState(TriggerState::HELD);

	auto stopCommand = std::make_unique<MovePlayerCommand>(player.get(), Engine::ValueCommand<Vector2>::InputType2D::ARROW_KEYS, 0.f);
	stopCommand->SetTriggerState(TriggerState::RELEASED);

	auto stopCommandController = std::make_unique<MovePlayerCommand>(player.get(), Engine::ValueCommand<Vector2>::InputType2D::D_PAD, 0.f);
	stopCommandController->SetTriggerState(TriggerState::RELEASED);

	InputManager::GetInstance().Bind2DValue(0, std::move(moveCommand));
	InputManager::GetInstance().Bind2DValue(0, std::move(stopCommand));
	InputManager::GetInstance().Bind2DValue(0, std::move(moveCommandController));
	InputManager::GetInstance().Bind2DValue(0, std::move(stopCommandController));

	auto playerGun{ std::make_unique<GameObject>() };
	 playerGun->AddComponent<ImageRenderer>("player_gun.png");
	//gunRenderer->ChangeImageAllignment(ImageAllignment::Centre);

	playerGun->GetTransform()->SetParent(player.get());
	playerGun->GetTransform()->SetLocalPosition(Vector2{ 0.f, 0.f });



	scene->Add(std::move(player));
	scene->Add(std::move(playerGun));
}
