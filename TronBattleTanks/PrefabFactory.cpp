#include "PrefabFactory.h"
#include "ImageRenderer.h"
#include "PlayerHealthComponent.h"
#include "StaticRotaterComponent.h"
#include "BoxCollider2D.h"
#include "CircleCollider.h"
#include "PhysicsBody.h"
#include "Vector2.h"
#include "Scene.h"
#include "Projectile.h"
#include "GameLoop.h"
#include "EnemyHealthComponent.h"
#include "EnemyMovement.h"
#include "EnemyBrain.h"

#include "InputManager.h"
#include "MovePlayerCommand.h"
#include "PlayerShootCommand.h"
#include "PlayerDamageCommand.h"
#include "PlayerAimCommand.h"

#define ENEMY_IMAGE_PATH "enemy_body.png"
#define UNIT_COLLIDER_SIZE Engine::Vector2{ 28, 28 }

using namespace Engine;


Engine::GameObject* PrefabFactory::AddPlayer(Scene* const scene, int playerIndex)
{
	auto player{ std::make_unique<GameObject>() };
	
	const auto playerBodyImagePath{ playerIndex == 0 ? "player_body.png" : "player_02_body.png" };
	auto playerImageRender{ player->AddComponent<ImageRenderer>(playerBodyImagePath) };
	playerImageRender->ChangeImageAllignment(ImageAllignment::Centre);

	player->AddComponent<PlayerHealthComponent>(1);

	auto collider{ player->AddComponent<BoxCollider2D>(UNIT_COLLIDER_SIZE, false) };
	collider->Center = Vector2{ -13.f, -13.f };
	collider->SetLayerMask(LayerMask::Player);
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

	auto playerDamageCommand = std::make_unique<PlayerDamageCommand>(player.get());
	playerDamageCommand->ChangeDeviceType(Engine::DeviceType::GAMEPAD);
	playerDamageCommand->SetTriggerState(TriggerState::PRESSED);

	InputManager::GetInstance().Bind2DValue(playerIndex, std::move(moveCommand));
	InputManager::GetInstance().Bind2DValue(playerIndex, std::move(stopCommand));
	InputManager::GetInstance().Bind2DValue(playerIndex, std::move(moveCommandController));
	InputManager::GetInstance().Bind2DValue(playerIndex, std::move(stopCommandController));
	InputManager::GetInstance().BindButton(playerIndex, 0x1000, std::move(playerDamageCommand)); // A button

	// Rotator
	auto playerGunRotator{ std::make_unique<GameObject>() };
	playerGunRotator->AddComponent<StaticRotaterComponent>(.1f, 2);
	playerGunRotator->GetTransform()->SetLocalPosition(Vector2{ -1.f, 0.f });
	playerGunRotator->GetTransform()->SetParent(player.get());

	// Bind Input
	auto aimCommand = std::make_unique<PlayerAimCommand>(playerGunRotator.get());
	aimCommand->ChangeDeviceType(Engine::DeviceType::GAMEPAD);
	aimCommand->SetTriggerState(TriggerState::CONSTANT);
	InputManager::GetInstance().Bind2DValue(playerIndex, std::move(aimCommand));

	//Gun
	const auto playerGunImagePath{ playerIndex == 0 ? "player_gun.png" : "player_02_gun.png" };
	auto playerGun{ std::make_unique<GameObject>() };
	auto gunRenderer{ playerGun->AddComponent<ImageRenderer>(playerGunImagePath) };
	gunRenderer->ChangeImageAllignment(ImageAllignment::Centre);
	gunRenderer->SetPivot(Vector2{ 0.f, 0.f });

	playerGun->GetTransform()->SetParent(playerGunRotator.get());
	playerGun->GetTransform()->SetLocalPosition(Vector2{ 8.f, 0.f });

	// Bind Input
	auto shootCommand = std::make_unique<PlayerShootCommand>(playerGun.get(),Vector2{20,0}, [scene]() { return AddPlayerBullet(scene); });
	shootCommand->ChangeDeviceType(Engine::DeviceType::GAMEPAD);
	shootCommand->SetTriggerState(TriggerState::PRESSED);
	InputManager::GetInstance().BindButton(playerIndex,0x4000,std::move(shootCommand));

	

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
	bullet->AddComponent<Projectile>(1, 6000.f, 3, LayerMask::Player);

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

Engine::GameObject* PrefabFactory::CreateEnemy(Engine::Scene* const scene, GameLoop* const gameLoop)
{
	auto enemyObject{ std::make_unique<GameObject>() };

	enemyObject->AddComponent<EnemyBrain>(8.f,gameLoop);
	enemyObject->AddComponent<EnemyHealthComponent>();
	enemyObject->AddComponent<EnemyMovement>(1000.f);

	auto imageRenderer{ enemyObject->AddComponent<ImageRenderer>(ENEMY_IMAGE_PATH) };
	imageRenderer->ChangeImageAllignment(ImageAllignment::Centre);
	auto collider{ enemyObject->AddComponent<BoxCollider2D>(UNIT_COLLIDER_SIZE) };
	collider->Center = Vector2{ -13.f, -13.f };
	enemyObject->AddComponent<PhysicsBody>();

	auto rawPtr{ enemyObject.get() };
	scene->Add(std::move(enemyObject));

	return rawPtr;
}

