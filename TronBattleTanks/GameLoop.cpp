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

#define MAP_01_POSITION Engine::Vector2{0, 72}
#define PLAYER_01_POSITION Engine::Vector2{ 128, 188 }
#define PLAYER_02_POSITION Engine::Vector2{ 228, 188 }

using namespace Engine;

GameLoop::GameLoop(Engine::GameObject* pOwner, GameMode mode, ScoreComponent* pScoreComponent) :
	Component(pOwner),
	m_pScoreComponent{ pScoreComponent },
	m_GameState{ GameState::Start },
	m_Mode{ mode }
{
	m_pPlayers = std::vector<Engine::GameObject*>{ nullptr,nullptr };
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
	if (m_pPlayerHealthComponent == sender)
	{
		EndGame();
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
	return m_pPlayers[randomPlayer];
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

	if (m_pPlayers[0] == nullptr)
	{
		m_pPlayers[0] = PrefabFactory::AddPlayer(pScene);
		m_pPlayers[0]->GetTransform()->SetWorldLocation(PLAYER_01_POSITION);
		m_pPlayerHealthComponent = m_pPlayers[0]->GetComponent<PlayerHealthComponent>();
		m_pPlayerHealthComponent->GetOnTakeDamage()->AddObserver(this);
	}
	else
	{
		m_pPlayers[0]->GetTransform()->SetWorldLocation(PLAYER_02_POSITION);
	}

	SpawnEnemies(pScene);
}

void GameLoop::CreatePVPPlayerLoop()
{
	auto pScene{ SceneManager::GetInstance().GetActiveScene() };

	m_pMapObject = PrefabFactory::Map1Parent(pScene);
	m_pMapObject->GetTransform()->SetWorldLocation(MAP_01_POSITION);

	if (m_pPlayers[0] == nullptr && m_pPlayers[1] == nullptr)
	{
		m_pPlayers[0] = PrefabFactory::AddPlayer(pScene);
		m_pPlayers[0]->GetTransform()->SetWorldLocation(PLAYER_01_POSITION);
		m_pPlayerHealthComponent = m_pPlayers[0]->GetComponent<PlayerHealthComponent>();
		m_pPlayerHealthComponent->GetOnTakeDamage()->AddObserver(this);

		m_pPlayers[1] = PrefabFactory::AddPlayer(pScene, 1);
		m_pPlayers[1]->GetTransform()->SetWorldLocation(PLAYER_02_POSITION);
		m_pPlayerHealthComponent = m_pPlayers[1]->GetComponent<PlayerHealthComponent>();
		m_pPlayerHealthComponent->GetOnTakeDamage()->AddObserver(this);
	}
	else
	{
		m_pPlayers[0]->GetTransform()->SetWorldLocation(PLAYER_01_POSITION);
		m_pPlayers[1]->GetTransform()->SetWorldLocation(PLAYER_02_POSITION);
	}
}

void GameLoop::CreateCo_OpPlayerLoop()
{
	auto pScene{ SceneManager::GetInstance().GetActiveScene() };

	m_pMapObject = PrefabFactory::Map1Parent(pScene);
	m_pMapObject->GetTransform()->SetWorldLocation(MAP_01_POSITION);

	if (m_pPlayers[0] == nullptr && m_pPlayers[1] == nullptr)
	{
		m_pPlayers[0] = PrefabFactory::AddPlayer(pScene);
		m_pPlayers[0]->GetTransform()->SetWorldLocation(PLAYER_01_POSITION);
		m_pPlayerHealthComponent = m_pPlayers[0]->GetComponent<PlayerHealthComponent>();
		m_pPlayerHealthComponent->GetOnTakeDamage()->AddObserver(this);

		m_pPlayers[1] = PrefabFactory::AddPlayer(pScene, 1);
		m_pPlayers[1]->GetTransform()->SetWorldLocation(PLAYER_02_POSITION);
		m_pPlayerHealthComponent = m_pPlayers[1]->GetComponent<PlayerHealthComponent>();
		m_pPlayerHealthComponent->GetOnTakeDamage()->AddObserver(this);
	}
	else
	{
		m_pPlayers[0]->GetTransform()->SetWorldLocation(PLAYER_01_POSITION);
		m_pPlayers[1]->GetTransform()->SetWorldLocation(PLAYER_02_POSITION);
	}

	SpawnEnemies(pScene);
}

void GameLoop::EndGame()
{
	m_GameState = GameState::GameOver;

	Destroy(m_pMapObject);

	m_pStartText->IsEnabled = true;
	m_pStartText->SetText("Game Over! Press START button to Restart");
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
		//Engine::Vector2{80,330},
		//Engine::Vector2{222,430},
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
