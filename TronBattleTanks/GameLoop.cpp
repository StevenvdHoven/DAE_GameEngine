#include "GameLoop.h"
#include "ScoreComponent.h"
#include "TextRenderer.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextRenderer.h"
#include "PrefabFactory.h"
#include "PlayerHealthComponent.h"
#include "MenuComponent.h"

using namespace Engine;

GameLoop::GameLoop(Engine::GameObject* pOwner, GameMode mode, ScoreComponent* pScoreComponent) :
	Component(pOwner),
	m_pScoreComponent{ pScoreComponent },
	m_GameState{GameState::Start},
	m_Mode{mode}
{
	m_pScoreComponent->GetOnScoreChange()->AddObserver(this);
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
	if (m_pScoreComponent == sender)
	{
		HandleScoreInfo();
	}
	else if (m_pPlayerHealthComponent == sender)
	{
		m_GameState = GameState::GameOver;
		
		Destroy(m_pMapObject);

		m_pStartText->IsEnabled = true;
		m_pStartText->SetText("Game Over! Press START button to Restart");
	}
}

void GameLoop::HandleScoreInfo()
{

}

void GameLoop::CreateStartText()
{
	Scene* pScene{ SceneManager::GetInstance().GetActiveScene() };

	auto pFont{ ResourceManager::GetInstance().LoadFont("Lingua.otf", 32) };
	auto startTextObject{ std::make_unique<GameObject>() };
	startTextObject->GetTransform()->SetLocalPosition(0, 220);
	m_pStartText = startTextObject->AddComponent<TextRenderer>("Press START button to Start", pFont);

	pScene->Add(std::move(startTextObject));
}

void GameLoop::CreateSinglePlayerLoop()
{
	auto pScene{ SceneManager::GetInstance().GetActiveScene() };

	m_pMapObject = PrefabFactory::Map1Parent(pScene);
	m_pMapObject->GetTransform()->SetWorldLocation(0, 72);

	if (m_pPlayers[0] == nullptr)
	{
		m_pPlayers[0] = PrefabFactory::AddPlayer(pScene);
		m_pPlayers[0]->GetTransform()->SetWorldLocation(128, 188);
		m_pPlayerHealthComponent = m_pPlayers[0]->GetComponent<PlayerHealthComponent>();
		m_pPlayerHealthComponent->GetOnTakeDamage()->AddObserver(this);
	}
	else
	{
		m_pPlayers[0]->GetTransform()->SetWorldLocation(128, 188);
	}
}

void GameLoop::CreatePVPPlayerLoop()
{
	auto pScene{ SceneManager::GetInstance().GetActiveScene() };

	m_pMapObject = PrefabFactory::Map1Parent(pScene);
	m_pMapObject->GetTransform()->SetWorldLocation(0, 72);

	if (m_pPlayers[0] == nullptr && m_pPlayers[1])
	{
		m_pPlayers[0] = PrefabFactory::AddPlayer(pScene);
		m_pPlayers[0]->GetTransform()->SetWorldLocation(128, 188);
		m_pPlayerHealthComponent = m_pPlayers[0]->GetComponent<PlayerHealthComponent>();
		m_pPlayerHealthComponent->GetOnTakeDamage()->AddObserver(this);

		m_pPlayers[1] = PrefabFactory::AddPlayer(pScene,1);
		m_pPlayers[1]->GetTransform()->SetWorldLocation(228, 188);
		m_pPlayerHealthComponent = m_pPlayers[1]->GetComponent<PlayerHealthComponent>();
		m_pPlayerHealthComponent->GetOnTakeDamage()->AddObserver(this);
	}
	else
	{
		m_pPlayers[0]->GetTransform()->SetWorldLocation(128, 188);
		m_pPlayers[1]->GetTransform()->SetWorldLocation(228, 188);
	}
}

void GameLoop::CreateCo_OpPlayerLoop()
{
	auto pScene{ SceneManager::GetInstance().GetActiveScene() };

	m_pMapObject = PrefabFactory::Map1Parent(pScene);
	m_pMapObject->GetTransform()->SetWorldLocation(0, 72);

	if (m_pPlayers[0] == nullptr && m_pPlayers[1] == nullptr)
	{
		m_pPlayers[0] = PrefabFactory::AddPlayer(pScene);
		m_pPlayers[0]->GetTransform()->SetWorldLocation(128, 188);
		m_pPlayerHealthComponent = m_pPlayers[0]->GetComponent<PlayerHealthComponent>();
		m_pPlayerHealthComponent->GetOnTakeDamage()->AddObserver(this);

		m_pPlayers[1] = PrefabFactory::AddPlayer(pScene,1);
		m_pPlayers[1]->GetTransform()->SetWorldLocation(228, 188);
		m_pPlayerHealthComponent = m_pPlayers[1]->GetComponent<PlayerHealthComponent>();
		m_pPlayerHealthComponent->GetOnTakeDamage()->AddObserver(this);
	}
	else
	{
		m_pPlayers[0]->GetTransform()->SetWorldLocation(128, 188);
		m_pPlayers[1]->GetTransform()->SetWorldLocation(228, 188);
	}
}
