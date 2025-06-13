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
#include "SkipSceneCommand.h"
#include "EnemyBrain.h"
#include "EngineTime.h"
#include <SDL.h>
#include "SimpleTriggerComponent.h"
#include "PlayerMovement.h"
#include <ImageRenderer.h>

#define MAP_01_POSITION Engine::Vector2{0, 72}
#define PLAYER_01_POSITION Engine::Vector2{ 128, 188 }
#define PLAYER_02_POSITION Engine::Vector2{ 228, 188 }
#define TEXT_TYPE_RATE 0.1f

#define PRESS_BUTTON 0x010

using namespace Engine;

GameLoop::GameLoop(Engine::GameObject* pOwner, GameMode mode, ScoreComponent* pScoreComponent) :
	Component(pOwner),
	m_RoundCount{ 0 },
	m_pScoreComponent{ pScoreComponent },
	m_CristalTrigger{ nullptr },
	m_SwitchMap{ false },
	m_SwitchDelay{ 0.f },
	m_pMapObjects{},
	m_GameMusic{ ServiceLocator::GetSoundSystem().LoadMusic("tron-music.wav") },
	m_CrystalClip{ ServiceLocator::GetSoundSystem().LoadSound("crystalsound.wav") },
	m_TypingTickClip{ ServiceLocator::GetSoundSystem().LoadSound("typing_tick.wav") },
	m_GameState{ GameState::Start },
	m_Mode{ mode }
{
	m_pPlayers = std::vector<PlayerState>{ PlayerState{},PlayerState{} };

	auto startGameCommand{ std::make_unique<StartGameCommand>(this) };
	startGameCommand->ChangeDeviceType(Engine::DeviceType::GAMEPAD);
	startGameCommand->SetTriggerState(Engine::TriggerState::PRESSED);

	auto keyboardStartCommand{ std::make_unique<StartGameCommand>(this) };
	keyboardStartCommand->ChangeDeviceType(Engine::DeviceType::KEYBOARD);
	keyboardStartCommand->SetTriggerState(Engine::TriggerState::PRESSED);

	auto skipSceneCommand{ std::make_unique<SkipSceneCommand>([this]()
		{
			GameOverScene::CreateScene(EGameOverType::LOST,m_Mode,0);
		}) };

	m_pStartGameCommand = startGameCommand.get();
	m_pKeyboardStartCommand = keyboardStartCommand.get();
	m_pSkipSceneCommand = skipSceneCommand.get();

	InputManager::GetInstance().BindButton(0, PRESS_BUTTON, std::move(startGameCommand));
	InputManager::GetInstance().BindButton(0, SDL_SCANCODE_Z, std::move(keyboardStartCommand));
	InputManager::GetInstance().BindButton(0, SDL_SCANCODE_F1, std::move(skipSceneCommand));

	if (mode == GameMode::VS)
	{
		pScoreComponent->GetGameObject()->SetActive(false);
	}
}

GameLoop::~GameLoop()
{
	ServiceLocator::GetSoundSystem().StopAll();
	InputManager::GetInstance().Unbind(0, m_pSkipSceneCommand);
	InputManager::GetInstance().Unbind(0, m_pStartGameCommand);
	InputManager::GetInstance().Unbind(0, m_pKeyboardStartCommand);
}

void GameLoop::Start()
{

	SpawnMaps();

	m_CristalTrigger = SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Cristal")->GetComponent<SimpleTriggerComponent>();
	m_CristalTrigger->GetOnTrigger().AddObserver(this);

	m_pInstructionText = SceneManager::GetInstance().GetActiveScene()->FindObjectByName("INSTRUCTION_TEXT")->GetComponent<TextRenderer>();

	CreateStartText();

	CreateLivesText();

	SetInstruction("Kill all enemies");
}

void GameLoop::Update()
{
	UpdateInstructionText();
	if (m_SwitchMap)
	{
		m_SwitchDelay -= Engine::Time::GetInstance().GetDeltaTime();
		if (m_SwitchDelay <= 0)
		{
			m_SwitchMap = false;
			NextMap();
			SpawnEnemies(SceneManager::GetInstance().GetActiveScene());

			auto pPlayer1Spawn{ m_pPlayerSpawns[m_CurrentMapIndex].first };
			auto pPlayer2Spawn{ m_pPlayerSpawns[m_CurrentMapIndex].second };


			for (int index{ 0 }; index < m_pPlayers.size(); ++index)
			{
				if (m_pPlayers[index].pPlayer == nullptr) continue;
				auto playerTransform{ m_pPlayers[index].pPlayer->GetTransform() };
				if (index == 0)
				{
					playerTransform->SetWorldLocation(pPlayer1Spawn->GetTransform()->GetWorldLocation());
				}
				else if (index == 1)
				{
					playerTransform->SetWorldLocation(pPlayer2Spawn->GetTransform()->GetWorldLocation());
				}

				auto playerMovement{ m_pPlayers[index].pPlayer->GetComponent<PlayerMovement>() };
				if (playerMovement)
				{
					playerMovement->EnableMovement();
				}
			}
		}
	}
}

void GameLoop::BeginGame()
{
	m_GameState = GameState::Running;
	InputManager::GetInstance().Unbind(0, m_pStartGameCommand);
	InputManager::GetInstance().Unbind(0, m_pKeyboardStartCommand);

	ServiceLocator::GetSoundSystem().PlayMusic(m_GameMusic);
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
	m_pInstructionText->IsEnabled = false;
}

void GameLoop::OnNotify(Component* sender)
{
	if (m_CristalTrigger == sender)
	{
		auto lastHit{ m_CristalTrigger->GetLastHit() };
		if (lastHit)
		{
			const auto randomPos{ GetRandomMapLocation() };
			lastHit->GetTransform()->SetWorldLocation(randomPos);
			ServiceLocator::GetSoundSystem().PlaySound(m_CrystalClip);
			return;
		}
	}

	std::vector<PlayerState>::iterator it;
	if (IsPlayerEvent(sender, it))
	{
		auto& player{ *it };
		player.Lives--;
		player.pTextComp->SetText("P" + std::to_string(player.Index) + " Lives " + std::to_string(player.Lives));


		if (player.Lives <= 0)
		{

			if (IsGameOver())
			{
				EndGame();
				return;
			}
			

			Destroy(player.pPlayer);

			UpdateEnemies();

			
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
	auto playerObject{ m_pPlayers[randomPlayer] };
	while (playerObject.pPlayer == nullptr || playerObject.pPlayer->IsDestroyed())
	{
		playerObject = m_pPlayers[static_cast<int>(std::rand() % m_pPlayers.size())];
	}

	return playerObject.pPlayer;
}

std::string GameLoop::GetTypeName() const
{
	return "GameLoop";
}

bool GameLoop::IsPlayerEvent(Component* pSender, auto& iterator)
{
	iterator = std::find_if(m_pPlayers.begin(), m_pPlayers.end(), [pSender](PlayerState state)
		{
			return state.pHealthComp == pSender;
		});
	return iterator != m_pPlayers.end();
}

bool GameLoop::IsAllPlayersDead()
{
	return false;
}

void GameLoop::UpdateEnemies()
{
	for (auto& enemy : m_pSpawnedEnemies)
	{
		EnemyBrain* pEnemyBrain{ enemy->GetComponent<EnemyBrain>() };
		pEnemyBrain->RefreshPlayer();
	}
}

void GameLoop::SpawnMaps()
{
	m_pMapObjects.resize(3);
	m_pGraphs.resize(3);
	auto pScene{ SceneManager::GetInstance().GetActiveScene() };

	for (int index{ 0 }; index < m_pMapObjects.size(); ++index)
	{
		const std::string prefabName{ "MAP0" + std::to_string(index + 1) + ".json" };
		const std::string graphName{ "map_0" + std::to_string(index + 1) + ".json" };

		auto graph{ ServiceLocator::GetPathFinding().GetGraph(graphName) };

		auto result{ Engine::EnginePrefabFactory::LoadPrefabs(prefabName) };
		if (result.bSuccesfull) m_pMapObjects[index] = result.Parent.get();
		m_pGraphs[index] = std::move(graph);

		Engine::EnginePrefabFactory::AddPrefabToScene(std::move(result), pScene);
		m_pMapObjects[index]->GetTransform()->SetWorldLocation(Engine::Vector2{ 1000, 1000 });

		std::vector<Engine::GameObject*> playerSpawns;
		playerSpawns.resize(2);
		for (int childIndex{ 0 }; childIndex < playerSpawns.size(); ++childIndex)
		{
			const std::string childName{ "PLAYER" + std::to_string(childIndex + 1) + "_SPAWN" };
			playerSpawns[childIndex] = m_pMapObjects[index]->GetTransform()->FindObjectByNameInChilderen(childName);
			playerSpawns[childIndex]->GetComponent<Engine::ImageRenderer>()->IsEnabled = false;
		}
		m_pPlayerSpawns[index] = std::make_pair(playerSpawns[0], playerSpawns[1]);
	}
}

void GameLoop::CreateStartText()
{
	Scene* pScene{ SceneManager::GetInstance().GetActiveScene() };

	auto startTextObject{ std::make_unique<GameObject>() };

	const Engine::Vector2 startTextObjectPosition{ 20,220 };
	startTextObject->GetTransform()->SetLocalPosition(startTextObjectPosition);
	m_pStartText = startTextObject->AddComponent<TextRenderer>("Press START button to Start", "tron-arcade.otf", 16, Engine::Color{ 0,0,255,255 });

	pScene->Add(std::move(startTextObject));
}

void GameLoop::CreateLivesText()
{
	auto pScene{ SceneManager::GetInstance().GetActiveScene() };

	const int amountOfPlayers{ m_Mode == GameMode::SinglePlayer ? 1 : 2 };
	constexpr float spacing{ 20.f };
	m_pLivesTexts.resize(amountOfPlayers);
	for (int index{ 0 }; index < amountOfPlayers; ++index)
	{
		auto livesObject{ std::make_unique<Engine::GameObject>() };
		livesObject->GetTransform()->SetLocalPosition(250.f, index * spacing);
		m_pLivesTexts[index] = livesObject->AddComponent<TextRenderer>("P" + std::to_string(index) + " Lives", "tron-arcade.otf", 20, Engine::Color{ 0,0,255,255 });

		pScene->Add(std::move(livesObject));
	}
}

void GameLoop::CreateSinglePlayerLoop()
{
	auto pScene{ SceneManager::GetInstance().GetActiveScene() };

	m_pMapObjects[0]->GetTransform()->SetWorldLocation(MAP_01_POSITION);

	auto playerSpawn{ m_pPlayerSpawns[m_CurrentMapIndex].first };

	SpawnPlayer(0, playerSpawn->GetTransform()->GetWorldLocation(), pScene);

	SpawnEnemies(pScene);
}

void GameLoop::CreatePVPPlayerLoop()
{
	auto pScene{ SceneManager::GetInstance().GetActiveScene() };

	m_pMapObjects[0]->GetTransform()->SetWorldLocation(MAP_01_POSITION);

	// Spawn players at their respective spawn points
	auto playerSpawn1{ m_pPlayerSpawns[m_CurrentMapIndex].first };
	auto playerSpawn2{ m_pPlayerSpawns[m_CurrentMapIndex].second };

	SpawnPlayer(0, playerSpawn1->GetTransform()->GetWorldLocation(), pScene);
	SpawnPlayer(1, playerSpawn2->GetTransform()->GetWorldLocation(), pScene);
}

void GameLoop::CreateCo_OpPlayerLoop()
{
	auto pScene{ SceneManager::GetInstance().GetActiveScene() };

	m_pMapObjects[0]->GetTransform()->SetWorldLocation(MAP_01_POSITION);

	// Spawn players at their respective spawn points
	auto playerSpawn1{ m_pPlayerSpawns[m_CurrentMapIndex].first };
	auto playerSpawn2{ m_pPlayerSpawns[m_CurrentMapIndex].second };

	SpawnPlayer(0, playerSpawn1->GetTransform()->GetWorldLocation(), pScene);
	SpawnPlayer(1, playerSpawn2->GetTransform()->GetWorldLocation(), pScene);

	SpawnEnemies(pScene);
}

Engine::Vector2 GameLoop::GetRandomMapLocation() const
{
	auto pGraph{ m_pGraphs[m_CurrentMapIndex] };
	const auto& allNodes{ pGraph->GetNodes() };

	int index{ static_cast<int>(rand() % allNodes.size()) };

	return allNodes[index]->Position;
}

void GameLoop::EndGame()
{
	InputManager::GetInstance().Unbind(0, m_pStartGameCommand);
	switch (m_Mode)
	{
	case GameMode::SinglePlayer:
		GameOverScene::CreateScene(EGameOverType::LOST, m_Mode, m_pScoreComponent->GetScore());
		break;
	case GameMode::CoOp:
		GameOverScene::CreateScene(EGameOverType::LOST, m_Mode, m_pScoreComponent->GetScore());
		break;
	case GameMode::VS:
		if (m_pPlayers[0].Lives > 0) GameOverScene::CreateScene(EGameOverType::PLAYER1WON, m_Mode);
		else GameOverScene::CreateScene(EGameOverType::PLAYER2WON, m_Mode);
		break;
	}
}

void GameLoop::NextRound()
{
	for (auto& player : m_pPlayers)
	{
		if (player.pPlayer)
		{
			auto playerMovement{ player.pPlayer->GetComponent<PlayerMovement>() };
			if (playerMovement)
			{
				playerMovement->DisableMovement();
			}
		}
	}

	m_RoundCount++;
	m_pMapObjects[m_CurrentMapIndex]->GetTransform()->SetWorldLocation(1000, 1000);
	m_SwitchMap = true;
	m_SwitchDelay = 3.f;

	std::vector<std::string> possibleInstructions
	{
		"You are not done yet",
		"Another round",
		"Keep going",
		"Dont give up",
	};
	int randomIndex{ static_cast<int>(std::rand() % possibleInstructions.size()) };
	SetInstruction(possibleInstructions[randomIndex]);
}

void GameLoop::SetInstruction(const std::string& instruction)
{
	m_InstructionText = "";
	m_TargetInstructionText = instruction;
	m_InstructionTextIndex = 0;

	m_pInstructionText->IsEnabled = true;
	m_pInstructionText->SetText(" ");
	m_InstructionTextTimer = TEXT_TYPE_RATE;
}

void GameLoop::UpdateInstructionText()
{
	if (m_InstructionTextIndex < m_TargetInstructionText.size())
	{
		m_InstructionTextTimer -= Engine::Time::GetInstance().GetDeltaTime();
		if (m_InstructionTextTimer <= 0.f)
		{
			m_InstructionText += m_TargetInstructionText[m_InstructionTextIndex];
			ServiceLocator::GetSoundSystem().PlaySound(m_TypingTickClip);
			m_pInstructionText->SetText(m_InstructionText);
			m_InstructionTextIndex++;
			m_InstructionTextTimer = TEXT_TYPE_RATE;
		}
	}
}

void GameLoop::NextMap()
{
	++m_CurrentMapIndex;
	if (m_CurrentMapIndex >= m_pMapObjects.size())
	{
		m_CurrentMapIndex = 0;
	}
	m_pMapObjects[m_CurrentMapIndex]->GetTransform()->SetWorldLocation(MAP_01_POSITION);
}

void GameLoop::SpawnEnemies(Engine::Scene* const pScene)
{
	m_pSpawnedEnemies.clear();
	std::vector<Engine::Vector2> spawnPositions;
	int amountOfEnemies{ GetAmountOfEnemies() };
	for (int i{ 0 }; i < amountOfEnemies; ++i)
	{
		Engine::Vector2 randomPos{ GetRandomMapLocation() };
		while (std::find(spawnPositions.begin(), spawnPositions.end(), randomPos) != spawnPositions.end())
		{
			// If the position is already taken, get a new random position
			randomPos = GetRandomMapLocation();
		}
		spawnPositions.emplace_back(randomPos);
	}


	for (const auto& spawnPos : spawnPositions)
	{
		auto type{ GetRandomEnemyType() };
		auto enemy{ PrefabFactory::CreateEnemy(pScene,type,m_pGraphs[m_CurrentMapIndex],this) };
		enemy->GetTransform()->SetWorldLocation(spawnPos);
		m_pSpawnedEnemies.emplace_front(enemy);
		enemy->GetComponent<EnemyHealthComponent>()->OnTakeDamage().AddObserver(this);
	}
}

void GameLoop::SpawnPlayer(int index, const Engine::Vector2& pos, Engine::Scene* const pScene)
{
	if (m_pPlayers[index].pPlayer == nullptr)
	{
		m_pPlayers[index].Index = index;
		m_pPlayers[index].pPlayer = PrefabFactory::AddPlayer(pScene, index, m_Mode);
		m_pPlayers[index].pPlayer->GetTransform()->SetWorldLocation(pos);
		m_pPlayers[index].pHealthComp = m_pPlayers[index].pPlayer->GetComponent<PlayerHealthComponent>();
		m_pPlayers[index].pHealthComp->GetOnTakeDamage()->AddObserver(this);
		m_pPlayers[index].Lives = 1;
		m_pPlayers[index].pTextComp = m_pLivesTexts[index];
		m_pPlayers[index].pTextComp->SetText("P" + std::to_string(index) + " Lives " + std::to_string(m_pPlayers[index].Lives));
	}
}

bool GameLoop::IsGameOver()
{
	if (m_Mode == GameMode::SinglePlayer || m_Mode == GameMode::CoOp)
	{
		return std::all_of(m_pPlayers.begin(), m_pPlayers.end(), [](const PlayerState& player) { return player.Lives <= 0; });
	}
	else if (m_Mode == GameMode::VS)
	{
		return std::any_of(m_pPlayers.begin(), m_pPlayers.end(), [](const PlayerState& player) { return player.Lives <= 0; });
	}
	return false;
}

int GameLoop::GetAmountOfEnemies() const
{
	int count{ 0 };
	if (m_RoundCount < 3)
	{
		count = 2;
	}
	else if (m_RoundCount < 6)
	{
		count = 3;
	}
	else
	{
		count = 5;
	}
	return count;
}

EnemyType GameLoop::GetRandomEnemyType() const
{
	int percentage{ 0 };
	if (m_RoundCount < 3)
	{
		percentage = 50;
	}
	else if (m_RoundCount < 6)
	{
		percentage = 70;
	}
	else
	{
		percentage = 90;
	}
	int randomValue{ static_cast<int>(std::rand() % 100) };
	return randomValue < percentage ? EnemyType::TANK : EnemyType::RECOGNIZER;
}
