#include "PrefabFactory.h"
#include "ImageRenderer.h"
#include "PlayerHealthComponent.h"
#include "PlayerMovement.h"
#include "StaticRotaterComponent.h"
#include "BoxCollider2D.h"
#include "CircleCollider.h"
#include "PhysicsBody.h"
#include "Vector2.h"
#include "Scene.h"
#include "Projectile.h"
#include "GameLoop.h"
#include "InputUnbinder.h"
#include "EnemyHealthComponent.h"
#include "EnemyMovement.h"
#include "EnemyBrain.h"
#include <fstream>
#include <filesystem>

#include "InputManager.h"
#include "MovePlayerCommand.h"
#include "PlayerShootCommand.h"
#include "PlayerDamageCommand.h"
#include "PlayerAimCommand.h"
#include "EnemyShootCommand.h"
#include <SDL.h>

#define PREFAB_FILEPATH "../Data/Prefabs/"

#define ENEMY_TANK_IMAGE_PATH "enemy_tank_body.png"
#define ENEMY_RECONIZER_IMAGE_PATH "enemy_reconizer_body.png"
#define UNIT_COLLIDER_SIZE Engine::Vector2{ 25, 25 }
#define UNIT_SPEED 2000.0f
#define PROJECTILE_SPEED 10000.f

using namespace Engine;


Engine::GameObject* PrefabFactory::AddPlayer(Scene* const scene, int playerIndex)
{
	auto player{ std::make_unique<GameObject>() };
		
	const auto playerBodyImagePath{ playerIndex == 0 ? "player_body.png" : "player_02_body.png" };
	auto playerImageRender{ player->AddComponent<ImageRenderer>(playerBodyImagePath) };
	playerImageRender->ChangeImageAllignment(ImageAllignment::Centre);

	player->AddComponent<PlayerHealthComponent>();

	auto collider{ player->AddComponent<BoxCollider2D>(UNIT_COLLIDER_SIZE, false) };
	collider->Center = Vector2{ -12.5f, -12.5f };
	collider->SetLayerMask(LayerMask::Player);
	collider->GetIgnoreLayerMask().insert(LayerMask::Player);
	collider->GetIgnoreLayerMask().insert(LayerMask::Enemy);
	player->AddComponent<PhysicsBody>();
	player->AddComponent<PlayerMovement>(UNIT_SPEED);


	// Bind Input
	std::vector<void*> inputCommands;

	auto moveCommandController = std::make_unique<MovePlayerCommand>(player.get(), Engine::ValueCommand<Vector2>::InputType::D_PAD);
	moveCommandController->SetTriggerState(TriggerState::PRESSED);
	inputCommands.emplace_back(moveCommandController.get());
	InputManager::GetInstance().Bind2DValue(playerIndex, std::move(moveCommandController));
	

	auto stopCommandController = std::make_unique<MovePlayerCommand>(player.get(), Engine::ValueCommand<Vector2>::InputType::D_PAD);
	stopCommandController->SetTriggerState(TriggerState::RELEASED);
	inputCommands.emplace_back(stopCommandController.get());
	InputManager::GetInstance().Bind2DValue(playerIndex, std::move(stopCommandController));

	if (playerIndex == 0)
	{
		auto keyboardMoveCommand{ std::make_unique<MovePlayerCommand>(player.get(), Engine::ValueCommand<Vector2>::InputType::ARROW_KEYS) };
		keyboardMoveCommand->SetTriggerState(TriggerState::PRESSED);
		inputCommands.emplace_back(keyboardMoveCommand.get());
		InputManager::GetInstance().Bind2DValue(playerIndex, std::move(keyboardMoveCommand));

		auto keyboardStopCommand{ std::make_unique<MovePlayerCommand>(player.get(), Engine::ValueCommand<Vector2>::InputType::ARROW_KEYS) };
		keyboardStopCommand->SetTriggerState(TriggerState::RELEASED);
		inputCommands.emplace_back(keyboardStopCommand.get());
		InputManager::GetInstance().Bind2DValue(playerIndex, std::move(keyboardStopCommand));
	}

	// Rotator
	auto playerGunRotator{ std::make_unique<GameObject>() };
	playerGunRotator->AddComponent<StaticRotaterComponent>(.1f, 2);
	playerGunRotator->GetTransform()->SetLocalPosition(Vector2{ -1.f, 0.f });
	playerGunRotator->GetTransform()->SetParent(player.get());

	// Bind Input
	auto aimCommand = std::make_unique<PlayerAimCommand>(playerGunRotator.get());
	aimCommand->ChangeDeviceType(Engine::DeviceType::GAMEPAD);
	aimCommand->SetTriggerState(TriggerState::CONSTANT);
	inputCommands.emplace_back(aimCommand.get());

	InputManager::GetInstance().Bind2DValue(playerIndex, std::move(aimCommand));

	if (playerIndex == 0)
	{
		auto keyboarAim{ std::make_unique<PlayerAimCommand>(playerGunRotator.get()) };
		keyboarAim->SetInputType(Engine::ValueCommand<Engine::Vector2>::InputType::WASD);
		keyboarAim->SetTriggerState(TriggerState::CONSTANT);
		inputCommands.emplace_back(keyboarAim.get());
	}

	//Gun
	const auto playerGunImagePath{ playerIndex == 0 ? "player_gun.png" : "player_02_gun.png" };
	auto playerGun{ std::make_unique<GameObject>() };
	auto gunRenderer{ playerGun->AddComponent<ImageRenderer>(playerGunImagePath) };
	gunRenderer->ChangeImageAllignment(ImageAllignment::Centre);
	gunRenderer->SetPivot(Vector2{ 0.f, 0.f });

	playerGun->GetTransform()->SetParent(playerGunRotator.get());
	playerGun->GetTransform()->SetLocalPosition(Vector2{ 8.f, 0.f });

	// Bind Input
	auto shootCommand = std::make_unique<PlayerShootCommand>(playerGun.get(),Vector2{10,0}, [scene]() { return AddPlayerBullet(scene); });
	shootCommand->ChangeDeviceType(Engine::DeviceType::GAMEPAD);
	shootCommand->SetInputType(Engine::ValueCommand<float>::InputType::RIGHT_TRIGGER);
	shootCommand->SetTriggerState(TriggerState::PRESSED);
	inputCommands.emplace_back(shootCommand.get());
	InputManager::GetInstance().BindValue(playerIndex, -1, std::move(shootCommand));

	if (playerIndex == 0)
	{
		auto keyboardShoot{ std::make_unique<PlayerShootCommand>(playerGun.get(),Vector2{10,0}, [scene]() { return AddPlayerBullet(scene); }) };
		keyboardShoot->ChangeDeviceType(Engine::DeviceType::KEYBOARD);
		keyboardShoot->SetTriggerState(TriggerState::PRESSED);
		inputCommands.emplace_back(keyboardShoot.get());
		InputManager::GetInstance().BindValue(playerIndex, SDL_SCANCODE_SPACE, std::move(keyboardShoot));
	}
	
	player->AddComponent<InputUnbinder>(playerIndex,std::move(inputCommands));

	


	

	auto rawPtr = player.get();
	scene->Add(std::move(player));
	scene->Add(std::move(playerGunRotator));
	scene->Add(std::move(playerGun));
	return rawPtr;
}

Engine::GameObject* PrefabFactory::AddPlayerBullet(Engine::Scene* const scene)
{
	auto bullet{ std::make_unique<GameObject>() };
	bullet->GetTransform()->SetLocalPosition(0, 0);
	auto imageRender{ bullet->AddComponent<ImageRenderer>("player_bullet.png") };
	imageRender->ChangeImageAllignment(ImageAllignment::Centre);

	auto bulletCollider = bullet->AddComponent<CircleCollider>(5.f,true);
	bulletCollider->SetLayerMask(LayerMask::Projectile);
	bullet->AddComponent<PhysicsBody>();
	bullet->AddComponent<Projectile>(EProjectileTarget::ENEMY,1, PROJECTILE_SPEED, 5, 1.f, LayerMask::Player);

	auto rawPtr = bullet.get();
	scene->Add(std::move(bullet));
	return rawPtr;
}

Engine::GameObject* PrefabFactory::AddEnemyBullet(Engine::Scene* const scene)
{
	auto bullet{ std::make_unique<GameObject>() };
	bullet->GetTransform()->SetLocalPosition(0, 0);
	auto imageRender{ bullet->AddComponent<ImageRenderer>("enemy_bullet.png") };
	imageRender->ChangeImageAllignment(ImageAllignment::Centre);

	auto bulletCollider = bullet->AddComponent<CircleCollider>(5.f, true);
	bulletCollider->SetLayerMask(LayerMask::Projectile);
	bullet->AddComponent<PhysicsBody>();
	bullet->AddComponent<Projectile>(EProjectileTarget::PLAYER,1, PROJECTILE_SPEED, 0,10.f, LayerMask::Enemy);

	auto rawPtr = bullet.get();
	scene->Add(std::move(bullet));
	return rawPtr;
}

Engine::GameObject* PrefabFactory::Map1Parent(Engine::Scene* const scene)
{
	auto MapParent{ std::make_unique<GameObject>() };

	auto backgroundObject{ std::make_unique<GameObject>() };
	backgroundObject->GetTransform()->SetLocalPosition(0, 0);
	backgroundObject->GetTransform()->SetParent(MapParent.get());
	backgroundObject->AddComponent<ImageRenderer>("gameBackground.png");
	scene->Add(std::move(backgroundObject));

	auto pLeftWall{ std::make_unique<GameObject>() };
	pLeftWall->GetTransform()->SetLocalPosition(0, 0);
	pLeftWall->AddComponent<BoxCollider2D>(Vector2{ 16, 440 }, false, LayerMask::Wall);
	pLeftWall->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pLeftWall));

	auto pRightWall{ std::make_unique<GameObject>() };
	pRightWall->GetTransform()->SetLocalPosition(464, 0);
	pRightWall->AddComponent<BoxCollider2D>(Vector2{ 16, 440 }, false, LayerMask::Wall);
	pRightWall->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pRightWall));

	auto pTopWall{ std::make_unique<GameObject>() };
	pTopWall->GetTransform()->SetLocalPosition(0, 0);
	pTopWall->AddComponent<BoxCollider2D>(Vector2{ 480, 3 }, false, LayerMask::Wall);
	pTopWall->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pTopWall));

	auto pBottomWall{ std::make_unique<GameObject>() };
	pBottomWall->GetTransform()->SetLocalPosition(0, 424);
	pBottomWall->AddComponent<BoxCollider2D>(Vector2{ 480, 16 }, false, LayerMask::Wall);
	pBottomWall->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pBottomWall));

	auto pMediumCollider1{ std::make_unique<GameObject>() };
	pMediumCollider1->GetTransform()->SetLocalPosition(48, 34);
	pMediumCollider1->AddComponent<BoxCollider2D>(Vector2{ 16, 35 }, false, LayerMask::Wall);
	pMediumCollider1->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider1));

	auto pMediumCollider2{ std::make_unique<GameObject>() };
	pMediumCollider2->GetTransform()->SetLocalPosition(48, 100);
	pMediumCollider2->AddComponent<BoxCollider2D>(Vector2{ 16, 49 }, false, LayerMask::Wall);
	pMediumCollider2->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider2));

	auto pMediumCollider3{ std::make_unique<GameObject>() };
	pMediumCollider3->GetTransform()->SetLocalPosition(48, 181);
	pMediumCollider3->AddComponent<BoxCollider2D>(Vector2{ 16, 65 }, false, LayerMask::Wall);
	pMediumCollider3->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider3));

	auto pMediumCollider4{ std::make_unique<GameObject>() };
	pMediumCollider4->GetTransform()->SetLocalPosition(48, 278);
	pMediumCollider4->AddComponent<BoxCollider2D>(Vector2{ 16, 65 }, false, LayerMask::Wall);
	pMediumCollider4->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider4));

	auto pMediumCollider5{ std::make_unique<GameObject>() };
	pMediumCollider5->GetTransform()->SetLocalPosition(64, 278);
	pMediumCollider5->AddComponent<BoxCollider2D>(Vector2{ 48, 17 }, false, LayerMask::Wall);
	pMediumCollider5->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider5));

	auto pMediumCollider6{ std::make_unique<GameObject>() };
	pMediumCollider6->GetTransform()->SetLocalPosition(0, 375);
	pMediumCollider6->AddComponent<BoxCollider2D>(Vector2{ 64, 16 }, false, LayerMask::Wall);
	pMediumCollider6->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider6));

	auto pMediumCollider7{ std::make_unique<GameObject>() };
	pMediumCollider7->GetTransform()->SetLocalPosition(96, 35);
	pMediumCollider7->AddComponent<BoxCollider2D>(Vector2{ 48, 17 }, false, LayerMask::Wall);
	pMediumCollider7->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider7));

	auto pMediumCollider8{ std::make_unique<GameObject>() };
	pMediumCollider8->GetTransform()->SetLocalPosition(144, 35);
	pMediumCollider8->AddComponent<BoxCollider2D>(Vector2{ 16, 65 }, false, LayerMask::Wall);
	pMediumCollider8->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider8));

	auto pMediumCollider9{ std::make_unique<GameObject>() };
	pMediumCollider9->GetTransform()->SetLocalPosition(144, 181);
	pMediumCollider9->AddComponent<BoxCollider2D>(Vector2{ 16, 65 }, false, LayerMask::Wall);
	pMediumCollider9->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider9));

	auto pMediumCollider10{ std::make_unique<GameObject>() };
	pMediumCollider10->GetTransform()->SetLocalPosition(96, 375);
	pMediumCollider10->AddComponent<BoxCollider2D>(Vector2{ 48, 17 }, false, LayerMask::Wall);
	pMediumCollider10->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider10));

	auto pMediumCollider11{ std::make_unique<GameObject>() };
	pMediumCollider11->GetTransform()->SetLocalPosition(144, 326);
	pMediumCollider11->AddComponent<BoxCollider2D>(Vector2{ 64, 17 }, false, LayerMask::Wall);
	pMediumCollider11->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider11));

	auto pMediumCollider12{ std::make_unique<GameObject>() };
	pMediumCollider12->GetTransform()->SetLocalPosition(176, 375);
	pMediumCollider12->AddComponent<BoxCollider2D>(Vector2{ 32, 17 }, false, LayerMask::Wall);
	pMediumCollider12->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider12));

	auto pMediumCollider13{ std::make_unique<GameObject>() };
	pMediumCollider13->GetTransform()->SetLocalPosition(240, 84);
	pMediumCollider13->AddComponent<BoxCollider2D>(Vector2{ 48, 17 }, false, LayerMask::Wall);
	pMediumCollider13->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider13));

	auto pMediumCollider14{ std::make_unique<GameObject>() };
	pMediumCollider14->GetTransform()->SetLocalPosition(192, 181);
	pMediumCollider14->AddComponent<BoxCollider2D>(Vector2{ 16, 65 }, false, LayerMask::Wall);
	pMediumCollider14->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider14));

	auto pMediumCollider15{ std::make_unique<GameObject>() };
	pMediumCollider15->GetTransform()->SetLocalPosition(240, 132);
	pMediumCollider15->AddComponent<BoxCollider2D>(Vector2{ 48, 17 }, false, LayerMask::Wall);
	pMediumCollider15->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider15));

	auto pMediumCollider16{ std::make_unique<GameObject>() };
	pMediumCollider16->GetTransform()->SetLocalPosition(272, 181);
	pMediumCollider16->AddComponent<BoxCollider2D>(Vector2{ 16, 65 }, false, LayerMask::Wall);
	pMediumCollider16->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider16));

	auto pMediumCollider17{ std::make_unique<GameObject>() };
	pMediumCollider17->GetTransform()->SetLocalPosition(240, 278);
	pMediumCollider17->AddComponent<BoxCollider2D>(Vector2{ 48, 17 }, false, LayerMask::Wall);
	pMediumCollider17->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider17));

	auto pMediumCollider18{ std::make_unique<GameObject>() };
	pMediumCollider18->GetTransform()->SetLocalPosition(240, 326);
	pMediumCollider18->AddComponent<BoxCollider2D>(Vector2{ 48, 17 }, false, LayerMask::Wall);
	pMediumCollider18->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider18));

	auto pMediumCollider19{ std::make_unique<GameObject>() };
	pMediumCollider19->GetTransform()->SetLocalPosition(240, 375);
	pMediumCollider19->AddComponent<BoxCollider2D>(Vector2{ 48, 17 }, false, LayerMask::Wall);
	pMediumCollider19->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider19));

	auto pMediumCollider20{ std::make_unique<GameObject>() };
	pMediumCollider20->GetTransform()->SetLocalPosition(288, 35);
	pMediumCollider20->AddComponent<BoxCollider2D>(Vector2{ 96, 17 }, false, LayerMask::Wall);
	pMediumCollider20->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider20));

	auto pMediumCollider21{ std::make_unique<GameObject>() };
	pMediumCollider21->GetTransform()->SetLocalPosition(368, 84);
	pMediumCollider21->AddComponent<BoxCollider2D>(Vector2{ 16, 65 }, false, LayerMask::Wall);
	pMediumCollider21->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider21));

	auto pMediumCollider22{ std::make_unique<GameObject>() };
	pMediumCollider22->GetTransform()->SetLocalPosition(320, 132);
	pMediumCollider22->AddComponent<BoxCollider2D>(Vector2{ 48, 17 }, false, LayerMask::Wall);
	pMediumCollider22->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider22));

	auto pMediumCollider23{ std::make_unique<GameObject>() };
	pMediumCollider23->GetTransform()->SetLocalPosition(320, 181);
	pMediumCollider23->AddComponent<BoxCollider2D>(Vector2{ 16, 65 }, false, LayerMask::Wall);
	pMediumCollider23->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider23));

	auto pMediumCollider24{ std::make_unique<GameObject>() };
	pMediumCollider24->GetTransform()->SetLocalPosition(368, 229);
	pMediumCollider24->AddComponent<BoxCollider2D>(Vector2{ 16, 66 }, false, LayerMask::Wall);
	pMediumCollider24->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider24));

	auto pMediumCollider25{ std::make_unique<GameObject>() };
	pMediumCollider25->GetTransform()->SetLocalPosition(320, 278);
	pMediumCollider25->AddComponent<BoxCollider2D>(Vector2{ 48, 17 }, false, LayerMask::Wall);
	pMediumCollider25->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider25));

	auto pMediumCollider26{ std::make_unique<GameObject>() };
	pMediumCollider26->GetTransform()->SetLocalPosition(320, 375);
	pMediumCollider26->AddComponent<BoxCollider2D>(Vector2{ 96, 17 }, false, LayerMask::Wall);
	pMediumCollider26->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider26));

	auto pMediumCollider27{ std::make_unique<GameObject>() };
	pMediumCollider27->GetTransform()->SetLocalPosition(416, 3);
	pMediumCollider27->AddComponent<BoxCollider2D>(Vector2{ 16, 49 }, false, LayerMask::Wall);
	pMediumCollider27->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider27));

	auto pMediumCollider28{ std::make_unique<GameObject>() };
	pMediumCollider28->GetTransform()->SetLocalPosition(416, 132);
	pMediumCollider28->AddComponent<BoxCollider2D>(Vector2{ 16, 65 }, false, LayerMask::Wall);
	pMediumCollider28->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider28));

	auto pMediumCollider29{ std::make_unique<GameObject>() };
	pMediumCollider29->GetTransform()->SetLocalPosition(416, 327);
	pMediumCollider29->AddComponent<BoxCollider2D>(Vector2{ 16, 65 }, false, LayerMask::Wall);
	pMediumCollider29->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pMediumCollider29));

	auto pSmallCollider1{ std::make_unique<GameObject>() };
	pSmallCollider1->GetTransform()->SetLocalPosition(96, 84);
	pSmallCollider1->AddComponent<BoxCollider2D>(Vector2{ 16, 16 }, false, LayerMask::Wall);
	pSmallCollider1->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pSmallCollider1));

	auto pSmallCollider2{ std::make_unique<GameObject>() };
	pSmallCollider2->GetTransform()->SetLocalPosition(96, 132);
	pSmallCollider2->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider2->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pSmallCollider2));

	auto pSmallCollider3{ std::make_unique<GameObject>() };
	pSmallCollider3->GetTransform()->SetLocalPosition(96, 181);
	pSmallCollider3->AddComponent<BoxCollider2D>(Vector2{ 16, 16 }, false, LayerMask::Wall);
	pSmallCollider3->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pSmallCollider3));

	auto pSmallCollider4{ std::make_unique<GameObject>() };
	pSmallCollider4->GetTransform()->SetLocalPosition(96, 229);
	pSmallCollider4->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider4->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pSmallCollider4));

	auto pSmallCollider5{ std::make_unique<GameObject>() };
	pSmallCollider5->GetTransform()->SetLocalPosition(96, 326);
	pSmallCollider5->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider5->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pSmallCollider5));

	auto pSmallCollider6{ std::make_unique<GameObject>() };
	pSmallCollider6->GetTransform()->SetLocalPosition(144, 132);
	pSmallCollider6->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider6->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pSmallCollider6));

	auto pSmallCollider7{ std::make_unique<GameObject>() };
	pSmallCollider7->GetTransform()->SetLocalPosition(144, 278);
	pSmallCollider7->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider7->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pSmallCollider7));

	auto pSmallCollider8{ std::make_unique<GameObject>() };
	pSmallCollider8->GetTransform()->SetLocalPosition(192, 35);
	pSmallCollider8->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider8->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pSmallCollider8));

	auto pSmallCollider9{ std::make_unique<GameObject>() };
	pSmallCollider9->GetTransform()->SetLocalPosition(192, 84);
	pSmallCollider9->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider9->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pSmallCollider9));

	auto pSmallCollider10{ std::make_unique<GameObject>() };
	pSmallCollider10->GetTransform()->SetLocalPosition(192, 132);
	pSmallCollider10->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider10->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pSmallCollider10));

	auto pSmallCollider11{ std::make_unique<GameObject>() };
	pSmallCollider11->GetTransform()->SetLocalPosition(208, 181);
	pSmallCollider11->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider11->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pSmallCollider11));

	auto pSmallCollider12{ std::make_unique<GameObject>() };
	pSmallCollider12->GetTransform()->SetLocalPosition(208, 229);
	pSmallCollider12->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider12->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pSmallCollider12));

	auto pSmallCollider13{ std::make_unique<GameObject>() };
	pSmallCollider13->GetTransform()->SetLocalPosition(192, 278);
	pSmallCollider13->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider13->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pSmallCollider13));

	auto pSmallCollider14{ std::make_unique<GameObject>() };
	pSmallCollider14->GetTransform()->SetLocalPosition(240, 35);
	pSmallCollider14->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider14->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pSmallCollider14));

	auto pSmallCollider15{ std::make_unique<GameObject>() };
	pSmallCollider15->GetTransform()->SetLocalPosition(256, 181);
	pSmallCollider15->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider15->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pSmallCollider15));

	auto pSmallCollider16{ std::make_unique<GameObject>() };
	pSmallCollider16->GetTransform()->SetLocalPosition(256, 229);
	pSmallCollider16->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider16->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pSmallCollider16));

	auto pSmallCollider17{ std::make_unique<GameObject>() };
	pSmallCollider17->GetTransform()->SetLocalPosition(320, 84);
	pSmallCollider17->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider17->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pSmallCollider17));

	auto pSmallCollider18{ std::make_unique<GameObject>() };
	pSmallCollider18->GetTransform()->SetLocalPosition(320, 326);
	pSmallCollider18->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider18->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pSmallCollider18));

	auto pSmallCollider19{ std::make_unique<GameObject>() };
	pSmallCollider19->GetTransform()->SetLocalPosition(368, 181);
	pSmallCollider19->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider19->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pSmallCollider19));

	auto pSmallCollider20{ std::make_unique<GameObject>() };
	pSmallCollider20->GetTransform()->SetLocalPosition(368, 326);
	pSmallCollider20->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider20->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pSmallCollider20));

	auto pSmallCollider21{ std::make_unique<GameObject>() };
	pSmallCollider21->GetTransform()->SetLocalPosition(416, 84);
	pSmallCollider21->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider21->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pSmallCollider21));

	auto pSmallCollider22{ std::make_unique<GameObject>() };
	pSmallCollider22->GetTransform()->SetLocalPosition(416, 229);
	pSmallCollider22->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider22->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pSmallCollider22));

	auto pSmallCollider23{ std::make_unique<GameObject>() };
	pSmallCollider23->GetTransform()->SetLocalPosition(416, 278);
	pSmallCollider23->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider23->GetTransform()->SetParent(MapParent.get());
	scene->Add(std::move(pSmallCollider23));

	auto RawPtrParent{ MapParent.get() };
	scene->Add(std::move(MapParent));
	return RawPtrParent;
}

Engine::GameObject* PrefabFactory::Map2Parent(Engine::Scene* const scene)
{
	auto map2Parent{ std::make_unique<GameObject>() };

	auto backgroundObject{ std::make_unique<GameObject>() };
	backgroundObject->GetTransform()->SetLocalPosition(0, 0);
	backgroundObject->GetTransform()->SetParent(map2Parent.get());
	backgroundObject->AddComponent<ImageRenderer>("gameBackground2.png");
	scene->Add(std::move(backgroundObject));

	auto pLeftWall{ std::make_unique<GameObject>() };
	pLeftWall->GetTransform()->SetLocalPosition(0, 0);
	pLeftWall->AddComponent<BoxCollider2D>(Vector2{ 16, 440 }, false, LayerMask::Wall);
	pLeftWall->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pLeftWall));

	auto pRightWall{ std::make_unique<GameObject>() };
	pRightWall->GetTransform()->SetLocalPosition(464, 0);
	pRightWall->AddComponent<BoxCollider2D>(Vector2{ 16, 440 }, false, LayerMask::Wall);
	pRightWall->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pRightWall));

	auto pTopWall{ std::make_unique<GameObject>() };
	pTopWall->GetTransform()->SetLocalPosition(0, 0);
	pTopWall->AddComponent<BoxCollider2D>(Vector2{ 480, 3 }, false, LayerMask::Wall);
	pTopWall->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pTopWall));

	auto pBottomWall{ std::make_unique<GameObject>() };
	pBottomWall->GetTransform()->SetLocalPosition(0, 424);
	pBottomWall->AddComponent<BoxCollider2D>(Vector2{ 480, 16 }, false, LayerMask::Wall);
	pBottomWall->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pBottomWall));

	auto pMediumCollider1{ std::make_unique<GameObject>() };
	pMediumCollider1->GetTransform()->SetLocalPosition(48, 3);
	pMediumCollider1->AddComponent<BoxCollider2D>(Vector2{ 16, 49 }, false, LayerMask::Wall);
	pMediumCollider1->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider1));

	auto pMediumCollider2{ std::make_unique<GameObject>() };
	pMediumCollider2->GetTransform()->SetLocalPosition(48, 132);
	pMediumCollider2->AddComponent<BoxCollider2D>(Vector2{ 16, 65 }, false, LayerMask::Wall);
	pMediumCollider2->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider2));

	auto pMediumCollider3{ std::make_unique<GameObject>() };
	pMediumCollider3->GetTransform()->SetLocalPosition(48, 326);
	pMediumCollider3->AddComponent<BoxCollider2D>(Vector2{ 16, 49 }, false, LayerMask::Wall);
	pMediumCollider3->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider3));

	auto pMediumCollider4{ std::make_unique<GameObject>() };
	pMediumCollider4->GetTransform()->SetLocalPosition(48, 375);
	pMediumCollider4->AddComponent<BoxCollider2D>(Vector2{ 112, 17 }, false, LayerMask::Wall);
	pMediumCollider4->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider4));

	auto pMediumCollider5{ std::make_unique<GameObject>() };
	pMediumCollider5->GetTransform()->SetLocalPosition(96, 35);
	pMediumCollider5->AddComponent<BoxCollider2D>(Vector2{ 96, 17 }, false, LayerMask::Wall);
	pMediumCollider5->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider5));

	auto pMediumCollider6{ std::make_unique<GameObject>() };
	pMediumCollider6->GetTransform()->SetLocalPosition(96, 84);
	pMediumCollider6->AddComponent<BoxCollider2D>(Vector2{ 16, 48 }, false, LayerMask::Wall);
	pMediumCollider6->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider6));

	auto pMediumCollider7{ std::make_unique<GameObject>() };
	pMediumCollider7->GetTransform()->SetLocalPosition(96, 132);
	pMediumCollider7->AddComponent<BoxCollider2D>(Vector2{ 64, 17 }, false, LayerMask::Wall);
	pMediumCollider7->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider7));

	auto pMediumCollider8{ std::make_unique<GameObject>() };
	pMediumCollider8->GetTransform()->SetLocalPosition(96, 229);
	pMediumCollider8->AddComponent<BoxCollider2D>(Vector2{ 16, 66 }, false, LayerMask::Wall);
	pMediumCollider8->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider8));

	auto pMediumCollider9{ std::make_unique<GameObject>() };
	pMediumCollider9->GetTransform()->SetLocalPosition(112, 278);
	pMediumCollider9->AddComponent<BoxCollider2D>(Vector2{ 48, 17 }, false, LayerMask::Wall);
	pMediumCollider9->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider9));

	auto pMediumCollider10{ std::make_unique<GameObject>() };
	pMediumCollider10->GetTransform()->SetLocalPosition(144, 181);
	pMediumCollider10->AddComponent<BoxCollider2D>(Vector2{ 16, 65 }, false, LayerMask::Wall);
	pMediumCollider10->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider10));

	auto pMediumCollider11{ std::make_unique<GameObject>() };
	pMediumCollider11->GetTransform()->SetLocalPosition(192, 84);
	pMediumCollider11->AddComponent<BoxCollider2D>(Vector2{ 48, 16 }, false, LayerMask::Wall);
	pMediumCollider11->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider11));

	auto pMediumCollider12{ std::make_unique<GameObject>() };
	pMediumCollider12->GetTransform()->SetLocalPosition(192, 132);
	pMediumCollider12->AddComponent<BoxCollider2D>(Vector2{ 48, 17 }, false, LayerMask::Wall);
	pMediumCollider12->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider12));

	auto pMediumCollider13{ std::make_unique<GameObject>() };
	pMediumCollider13->GetTransform()->SetLocalPosition(192, 181);
	pMediumCollider13->AddComponent<BoxCollider2D>(Vector2{ 32, 16 }, false, LayerMask::Wall);
	pMediumCollider13->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider13));

	auto pMediumCollider14{ std::make_unique<GameObject>() };
	pMediumCollider14->GetTransform()->SetLocalPosition(192, 197);
	pMediumCollider14->AddComponent<BoxCollider2D>(Vector2{ 16, 32 }, false, LayerMask::Wall);
	pMediumCollider14->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider14));

	auto pMediumCollider15{ std::make_unique<GameObject>() };
	pMediumCollider15->GetTransform()->SetLocalPosition(192, 229);
	pMediumCollider15->AddComponent<BoxCollider2D>(Vector2{ 32, 17 }, false, LayerMask::Wall);
	pMediumCollider15->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider15));

	auto pMediumCollider16{ std::make_unique<GameObject>() };
	pMediumCollider16->GetTransform()->SetLocalPosition(192, 278);
	pMediumCollider16->AddComponent<BoxCollider2D>(Vector2{ 48, 17 }, false, LayerMask::Wall);
	pMediumCollider16->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider16));

	auto pMediumCollider17{ std::make_unique<GameObject>() };
	pMediumCollider17->GetTransform()->SetLocalPosition(192, 326);
	pMediumCollider17->AddComponent<BoxCollider2D>(Vector2{ 48, 17 }, false, LayerMask::Wall);
	pMediumCollider17->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider17));

	auto pMediumCollider18{ std::make_unique<GameObject>() };
	pMediumCollider18->GetTransform()->SetLocalPosition(192, 375);
	pMediumCollider18->AddComponent<BoxCollider2D>(Vector2{ 48, 17 }, false, LayerMask::Wall);
	pMediumCollider18->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider18));

	auto pMediumCollider19{ std::make_unique<GameObject>() };
	pMediumCollider19->GetTransform()->SetLocalPosition(320, 35);
	pMediumCollider19->AddComponent<BoxCollider2D>(Vector2{ 16, 65 }, false, LayerMask::Wall);
	pMediumCollider19->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider19));

	auto pMediumCollider20{ std::make_unique<GameObject>() };
	pMediumCollider20->GetTransform()->SetLocalPosition(320, 181);
	pMediumCollider20->AddComponent<BoxCollider2D>(Vector2{ 16, 65 }, false, LayerMask::Wall);
	pMediumCollider20->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider20));

	auto pMediumCollider21{ std::make_unique<GameObject>() };
	pMediumCollider21->GetTransform()->SetLocalPosition(272, 326);
	pMediumCollider21->AddComponent<BoxCollider2D>(Vector2{ 64, 17 }, false, LayerMask::Wall);
	pMediumCollider21->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider21));

	auto pMediumCollider22{ std::make_unique<GameObject>() };
	pMediumCollider22->GetTransform()->SetLocalPosition(336, 35);
	pMediumCollider22->AddComponent<BoxCollider2D>(Vector2{ 48, 17 }, false, LayerMask::Wall);
	pMediumCollider22->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider22));

	auto pMediumCollider23{ std::make_unique<GameObject>() };
	pMediumCollider23->GetTransform()->SetLocalPosition(336, 375);
	pMediumCollider23->AddComponent<BoxCollider2D>(Vector2{ 48, 17 }, false, LayerMask::Wall);
	pMediumCollider23->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider23));

	auto pMediumCollider24{ std::make_unique<GameObject>() };
	pMediumCollider24->GetTransform()->SetLocalPosition(416, 35);
	pMediumCollider24->AddComponent<BoxCollider2D>(Vector2{ 16, 33 }, false, LayerMask::Wall);
	pMediumCollider24->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider24));

	auto pMediumCollider25{ std::make_unique<GameObject>() };
	pMediumCollider25->GetTransform()->SetLocalPosition(416, 100);
	pMediumCollider25->AddComponent<BoxCollider2D>(Vector2{ 16, 49 }, false, LayerMask::Wall);
	pMediumCollider25->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider25));

	auto pMediumCollider26{ std::make_unique<GameObject>() };
	pMediumCollider26->GetTransform()->SetLocalPosition(416, 181);
	pMediumCollider26->AddComponent<BoxCollider2D>(Vector2{ 16, 65 }, false, LayerMask::Wall);
	pMediumCollider26->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider26));

	auto pMediumCollider27{ std::make_unique<GameObject>() };
	pMediumCollider27->GetTransform()->SetLocalPosition(368, 278);
	pMediumCollider27->AddComponent<BoxCollider2D>(Vector2{ 64, 17 }, false, LayerMask::Wall);
	pMediumCollider27->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider27));

	auto pMediumCollider28{ std::make_unique<GameObject>() };
	pMediumCollider28->GetTransform()->SetLocalPosition(416, 295);
	pMediumCollider28->AddComponent<BoxCollider2D>(Vector2{ 16, 48 }, false, LayerMask::Wall);
	pMediumCollider28->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider28));

	auto pMediumCollider29{ std::make_unique<GameObject>() };
	pMediumCollider29->GetTransform()->SetLocalPosition(416, 375);
	pMediumCollider29->AddComponent<BoxCollider2D>(Vector2{ 48, 17 }, false, LayerMask::Wall);
	pMediumCollider29->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider29));

	auto pMediumCollider30{ std::make_unique<GameObject>() };
	pMediumCollider30->GetTransform()->SetLocalPosition(256, 181);
	pMediumCollider30->AddComponent<BoxCollider2D>(Vector2{ 32, 16 }, false, LayerMask::Wall);
	pMediumCollider30->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider30));

	auto pMediumCollider31{ std::make_unique<GameObject>() };
	pMediumCollider31->GetTransform()->SetLocalPosition(272, 197);
	pMediumCollider31->AddComponent<BoxCollider2D>(Vector2{ 16, 32 }, false, LayerMask::Wall);
	pMediumCollider31->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider31));

	auto pMediumCollider32{ std::make_unique<GameObject>() };
	pMediumCollider32->GetTransform()->SetLocalPosition(256, 229);
	pMediumCollider32->AddComponent<BoxCollider2D>(Vector2{ 32, 17 }, false, LayerMask::Wall);
	pMediumCollider32->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider32));
	
	auto pMediumCollider33{ std::make_unique<GameObject>() };
	pMediumCollider33->GetTransform()->SetLocalPosition(272, 375);
	pMediumCollider33->AddComponent<BoxCollider2D>(Vector2{ 32, 17 }, false, LayerMask::Wall);
	pMediumCollider33->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider33));

	auto pSmallCollider01{ std::make_unique<GameObject>() };
	pSmallCollider01->GetTransform()->SetLocalPosition(48, 84);
	pSmallCollider01->AddComponent<BoxCollider2D>(Vector2{ 16, 16 }, false, LayerMask::Wall);
	pSmallCollider01->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pSmallCollider01));

	auto pSmallCollider02{ std::make_unique<GameObject>() };
	pSmallCollider02->GetTransform()->SetLocalPosition(48, 229);
	pSmallCollider02->AddComponent<BoxCollider2D>(Vector2{ 16, 16 }, false, LayerMask::Wall);
	pSmallCollider02->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pSmallCollider02));

	auto pSmallCollider03{ std::make_unique<GameObject>() };
	pSmallCollider03->GetTransform()->SetLocalPosition(48, 278);
	pSmallCollider03->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider03->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pSmallCollider03));

	auto pSmallCollider04{ std::make_unique<GameObject>() };
	pSmallCollider04->GetTransform()->SetLocalPosition(96, 181);
	pSmallCollider04->AddComponent<BoxCollider2D>(Vector2{ 16, 16 }, false, LayerMask::Wall);
	pSmallCollider04->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pSmallCollider04));

	auto pSmallCollider05{ std::make_unique<GameObject>() };
	pSmallCollider05->GetTransform()->SetLocalPosition(96, 326);
	pSmallCollider05->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider05->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pSmallCollider05));

	auto pSmallCollider06{ std::make_unique<GameObject>() };
	pSmallCollider06->GetTransform()->SetLocalPosition(144, 84);
	pSmallCollider06->AddComponent<BoxCollider2D>(Vector2{ 16, 16 }, false, LayerMask::Wall);
	pSmallCollider06->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pSmallCollider06));

	auto pSmallCollider07{ std::make_unique<GameObject>() };
	pSmallCollider07->GetTransform()->SetLocalPosition(144, 326);
	pSmallCollider07->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider07->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pSmallCollider07));

	auto pSmallCollider08{ std::make_unique<GameObject>() };
	pSmallCollider08->GetTransform()->SetLocalPosition(224, 35);
	pSmallCollider08->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider08->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pSmallCollider08));

	auto pSmallCollider09{ std::make_unique<GameObject>() };
	pSmallCollider09->GetTransform()->SetLocalPosition(272, 35);
	pSmallCollider09->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider09->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pSmallCollider09));

	auto pSmallCollider10{ std::make_unique<GameObject>() };
	pSmallCollider10->GetTransform()->SetLocalPosition(272, 84);
	pSmallCollider10->AddComponent<BoxCollider2D>(Vector2{ 16, 16 }, false, LayerMask::Wall);
	pSmallCollider10->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pSmallCollider10));

	auto pSmallCollider11{ std::make_unique<GameObject>() };
	pSmallCollider11->GetTransform()->SetLocalPosition(272, 132);
	pSmallCollider11->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider11->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pSmallCollider11));

	auto pSmallCollider12{ std::make_unique<GameObject>() };
	pSmallCollider12->GetTransform()->SetLocalPosition(272, 278);
	pSmallCollider12->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider12->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pSmallCollider12));

	auto pSmallCollider13{ std::make_unique<GameObject>() };
	pSmallCollider13->GetTransform()->SetLocalPosition(320, 132);
	pSmallCollider13->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider13->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pSmallCollider13));

	auto pSmallCollider14{ std::make_unique<GameObject>() };
	pSmallCollider14->GetTransform()->SetLocalPosition(320, 278);
	pSmallCollider14->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider14->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pSmallCollider14));

	auto pSmallCollider15{ std::make_unique<GameObject>() };
	pSmallCollider15->GetTransform()->SetLocalPosition(368, 84);
	pSmallCollider15->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider15->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pSmallCollider15));

	auto pSmallCollider16{ std::make_unique<GameObject>() };
	pSmallCollider16->GetTransform()->SetLocalPosition(368, 132);
	pSmallCollider16->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider16->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pSmallCollider16));

	auto pSmallCollider17{ std::make_unique<GameObject>() };
	pSmallCollider17->GetTransform()->SetLocalPosition(368, 181);
	pSmallCollider17->AddComponent<BoxCollider2D>(Vector2{ 16, 16 }, false, LayerMask::Wall);
	pSmallCollider17->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pSmallCollider17));

	auto pSmallCollider18{ std::make_unique<GameObject>() };
	pSmallCollider18->GetTransform()->SetLocalPosition(368, 229);
	pSmallCollider18->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider18->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pSmallCollider18));

	auto pSmallCollider19{ std::make_unique<GameObject>() };
	pSmallCollider19->GetTransform()->SetLocalPosition(368, 326);
	pSmallCollider19->AddComponent<BoxCollider2D>(Vector2{ 16, 17 }, false, LayerMask::Wall);
	pSmallCollider19->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pSmallCollider19));

	auto rawPtrParent{ map2Parent.get() };
	scene->Add(std::move(map2Parent));

	return rawPtrParent;
}

Engine::GameObject* PrefabFactory::Map3Parent(Engine::Scene* const scene)
{
	auto map2Parent{ std::make_unique<GameObject>() };

	auto backgroundObject{ std::make_unique<GameObject>() };
	backgroundObject->GetTransform()->SetLocalPosition(0, 0);
	backgroundObject->GetTransform()->SetParent(map2Parent.get());
	backgroundObject->AddComponent<ImageRenderer>("gameBackground3.png");
	scene->Add(std::move(backgroundObject));

	auto pLeftWall{ std::make_unique<GameObject>() };
	pLeftWall->GetTransform()->SetLocalPosition(0, 0);
	pLeftWall->AddComponent<BoxCollider2D>(Vector2{ 16, 440 }, false, LayerMask::Wall);
	pLeftWall->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pLeftWall));

	auto pRightWall{ std::make_unique<GameObject>() };
	pRightWall->GetTransform()->SetLocalPosition(464, 0);
	pRightWall->AddComponent<BoxCollider2D>(Vector2{ 16, 440 }, false, LayerMask::Wall);
	pRightWall->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pRightWall));

	auto pTopWall{ std::make_unique<GameObject>() };
	pTopWall->GetTransform()->SetLocalPosition(0, 0);
	pTopWall->AddComponent<BoxCollider2D>(Vector2{ 480, 3 }, false, LayerMask::Wall);
	pTopWall->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pTopWall));

	auto pBottomWall{ std::make_unique<GameObject>() };
	pBottomWall->GetTransform()->SetLocalPosition(0, 424);
	pBottomWall->AddComponent<BoxCollider2D>(Vector2{ 480, 16 }, false, LayerMask::Wall);
	pBottomWall->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pBottomWall));

	auto pMediumCollider1{ std::make_unique<GameObject>() };
	pMediumCollider1->GetTransform()->SetLocalPosition(48, 35);
	pMediumCollider1->AddComponent<BoxCollider2D>(Vector2{ 112, 17 }, false, LayerMask::Wall);
	pMediumCollider1->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider1));

	auto pMediumCollider2{ std::make_unique<GameObject>() };
	pMediumCollider2->GetTransform()->SetLocalPosition(48, 52);
	pMediumCollider2->AddComponent<BoxCollider2D>(Vector2{ 16, 49 }, false, LayerMask::Wall);
	pMediumCollider2->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider2));

	auto pMediumCollider3{ std::make_unique<GameObject>() };
	pMediumCollider3->GetTransform()->SetLocalPosition(48, 229);
	pMediumCollider3->AddComponent<BoxCollider2D>(Vector2{ 16, 66 }, false, LayerMask::Wall);
	pMediumCollider3->GetTransform()->SetParent(map2Parent.get());
	scene->Add(std::move(pMediumCollider3));

	return nullptr;
}

Engine::GameObject* PrefabFactory::CreateEnemy(Engine::Scene* const scene, EnemyType enemyType, GameLoop* const gameLoop)
{
	auto enemyObject{ std::make_unique<GameObject>() };

	auto shootCommand{ std::make_unique<EnemyShootCommand>(enemyObject.get(),Engine::Vector2{0,0},[scene]() { return AddEnemyBullet(scene); })};

	enemyObject->AddComponent<EnemyBrain>(enemyType,2.f,4.f,gameLoop,std::move(shootCommand));
	enemyObject->AddComponent<EnemyHealthComponent>();
	enemyObject->AddComponent<EnemyMovement>(UNIT_SPEED);

	const std::string enemySpritePath{ enemyType == EnemyType::TANK ? ENEMY_TANK_IMAGE_PATH : ENEMY_RECONIZER_IMAGE_PATH };
	auto imageRenderer{ enemyObject->AddComponent<ImageRenderer>(enemySpritePath) };
	imageRenderer->ChangeImageAllignment(ImageAllignment::Centre);
	auto collider{ enemyObject->AddComponent<BoxCollider2D>(UNIT_COLLIDER_SIZE) };
	collider->SetLayerMask(LayerMask::Enemy);
	collider->GetIgnoreLayerMask().insert(LayerMask::Player);
	collider->GetIgnoreLayerMask().insert(LayerMask::Enemy);
	collider->Center = Vector2{ -13.f, -13.f };
	enemyObject->AddComponent<PhysicsBody>();

	auto rawPtr{ enemyObject.get() };
	scene->Add(std::move(enemyObject));

	return rawPtr;
}
