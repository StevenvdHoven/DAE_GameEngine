#include "GameLoop.h"
#include "ScoreComponent.h"
#include "TextRenderer.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextRenderer.h"
#include "PrefabFactory.h"
#include "PlayerHealthComponent.h"
#include "EnemyHealthComponent.h"
#include "MenuComponent.h"
#include "InputManager.h"
#include "ServiceLocator.h"
#include "PathFinding.h"
#include "GameOverScene.h"

#define MAP_01_POSITION Engine::Vector2{0, 72}
#define PLAYER_01_POSITION Engine::Vector2{ 128, 188 }
#define PLAYER_02_POSITION Engine::Vector2{ 228, 188 }

#define START_BUTTON 0x010

using namespace Engine;

GameLoop::GameLoop(Engine::GameObject* pOwner, GameMode mode, ScoreComponent* pScoreComponent) :
	Component(pOwner),
	m_pScoreComponent{ pScoreComponent },
	m_GameState{ GameState::Start },
	m_Mode{ mode }
{
	m_pPlayers = std::vector<PlayerState>{ PlayerState{},PlayerState{} };

	auto startGameCommand{ std::make_unique<StartGameCommand>(this) };
	startGameCommand->ChangeDeviceType(Engine::DeviceType::GAMEPAD);
	startGameCommand->SetTriggerState(Engine::TriggerState::PRESSED);

	m_pStarGameCommand = startGameCommand.get();
	InputManager::GetInstance().BindButton(0, START_BUTTON, std::move(startGameCommand));
}

void GameLoop::Start()
{
	CreateStartText();
}

void GameLoop::BeginGame()
{
	m_GameState = GameState::Running;

	//Spawn Player
	switch (m_Mode)
	{
	case GameMode::SinglePlayer:
		CreateSinglePlayerLoop();
		break;
	case GameMode::CoOp:
		CreateCo_OpPlayerLoop();
		break;
	case GameMode::VS:
		CreatePVPPlayerLoop();
		break;
	}

	m_pStartText->IsEnabled = false;
}

void GameLoop::OnNotify(Component* sender)
{
	std::vector<PlayerState>::iterator it;
	if (IsPlayerEvent(sender,it))
	{
		auto& player{ *it };
		player.Lives--;
		if (player.Lives <= 0)
		{
			EndGame();
		}
		else
		{
			const auto randomPos{ GetRandomMapLocation() };
			player.pPlayer->GetTransform()->SetWorldLocation(randomPos);
		}
	}
	else
	{
		auto enemyComponent{ sender->GetGameObject()->GetComponent<EnemyHealthComponent>() };
		if (enemyComponent && enemyComponent->IsDead())
		{
			m_pScoreComponent->AddScore(100);
			m_pSpawnedEnemies.remove(sender->GetGameObject());
		}

		int count{ 0 };
		std::for_each(m_pSpawnedEnemies.begin(), m_pSpawnedEnemies.end(), [&count](Engine::GameObject*)
			{
				++count;
			});

		if (count == 0)
			NextRound();
	}

}

Engine::GameObject* const GameLoop::GetRandomPlayer() const
{
	const int randomPlayer{ static_cast<int>(std::rand() % m_pPlayers.size()) };
	return m_pPlayers[randomPlayer].pPlayer;
}

bool GameLoop::IsPlayerEvent(Component* pSender, auto& iterator)
{
	iterator = std::find_if(m_pPlayers.begin(), m_pPlayers.end(), [pSender](PlayerState state)
		{
			return state.pHealthComp == pSender;
		});
	return iterator != m_pPlayers.end();
}

void GameLoop::CreateStartText()
{
	Scene* pScene{ SceneManager::GetInstance().GetActiveScene() };

	auto pFont{ ResourceManager::GetInstance().LoadFont("tron-arcade.otf", 16) };
	auto startTextObject{ std::make_unique<GameObject>() };

	const Engine::Vector2 startTextObjectPosition{ 20,220 };
	startTextObject->GetTransform()->SetLocalPosition(startTextObjectPosition);
	m_pStartText = startTextObject->AddComponent<TextRenderer>("Press START button to Start", pFont, Engine::Color{ 0,0,255,255 });

	pScene->Add(std::move(startTextObject));
}

void GameLoop::CreateSinglePlayerLoop()
{
	auto pScene{ SceneManager::GetInstance().GetActiveScene() };

	m_pMapObject = PrefabFactory::Map1Parent(pScene);
	m_pMapObject->GetTransform()->SetWorldLocation(MAP_01_POSITION);

	SpawnPlayer(0, PLAYER_01_POSITION, pScene);

	SpawnEnemies(pScene);
}

void GameLoop::CreatePVPPlayerLoop()
{
	auto pScene{ SceneManager::GetInstance().GetActiveScene() };

	m_pMapObject = PrefabFactory::Map1Parent(pScene);
	m_pMapObject->GetTransform()->SetWorldLocation(MAP_01_POSITION);

	SpawnPlayer(0, PLAYER_01_POSITION, pScene);
	SpawnPlayer(1, PLAYER_02_POSITION, pScene);
}

void GameLoop::CreateCo_OpPlayerLoop()
{
	auto pScene{ SceneManager::GetInstance().GetActiveScene() };

	m_pMapObject = PrefabFactory::Map1Parent(pScene);
	m_pMapObject->GetTransform()->SetWorldLocation(MAP_01_POSITION);

	SpawnPlayer(0, PLAYER_01_POSITION, pScene);
	SpawnPlayer(1, PLAYER_02_POSITION, pScene);

	SpawnEnemies(pScene);
}

Engine::Vector2 GameLoop::GetRandomMapLocation() const
{
	auto pGraph{ ServiceLocator::GetPathFinding().GetGraph("map_01.json") };
	const auto& allNodes{ pGraph->GetNodes() };

	int index{ static_cast<int>(rand() % allNodes.size()) };

	return allNodes[index]->Position;
}

void GameLoop::EndGame()
{
	InputManager::GetInstance().Unbind(0,m_pStarGameCommand);
	switch (m_Mode)
	{
	case GameMode::SinglePlayer:
		GameOverScene::CreateScene(EGameOverType::LOST);
		break;
	case GameMode::CoOp:
		GameOverScene::CreateScene(EGameOverType::LOST);
		break;
	case GameMode::VS:
		if (m_pPlayers[0].Lives > 0) GameOverScene::CreateScene(EGameOverType::PLAYER1WON);
		else GameOverScene::CreateScene(EGameOverType::PLAYER2WON);
		break; 
	}
}

void GameLoop::NextRound()
{
	SpawnEnemies(SceneManager::GetInstance().GetActiveScene());
}

void GameLoop::SpawnEnemies(Engine::Scene* const pScene)
{
	m_pSpawnedEnemies.clear();
	const std::vector<Engine::Vector2> spawnPositions
	{
		Engine::Vector2{80,330},
		Engine::Vector2{222,430},
		Engine::Vector2{400,242}
	};

	for (const auto& spawnPos : spawnPositions)
	{
		auto enemy{ PrefabFactory::CreateEnemy(pScene,this) };
		enemy->GetTransform()->SetWorldLocation(spawnPos);
		m_pSpawnedEnemies.emplace_front(enemy);
		enemy->GetComponent<EnemyHealthComponent>()->OnTakeDamage().AddObserver(this);
	}
}

void GameLoop::SpawnPlayer(int index, const Engine::Vector2& pos, Engine::Scene* const pScene)
{
	if (m_pPlayers[index].pPlayer == nullptr)
	{
		m_pPlayers[index].pPlayer = PrefabFactory::AddPlayer(pScene);
		m_pPlayers[index].pPlayer->GetTransform()->SetWorldLocation(pos);
		m_pPlayers[index].pHealthComp = m_pPlayers[index].pPlayer->GetComponent<PlayerHealthComponent>();
		m_pPlayers[index].pHealthComp->GetOnTakeDamage()->AddObserver(this);
		m_pPlayers[index].Lives = 3;
	}
}
