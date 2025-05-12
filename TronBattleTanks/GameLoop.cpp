#include "GameLoop.h"
#include "ScoreComponent.h"
#include "TextRenderer.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextRenderer.h"
#include "PrefabFactory.h"
#include "PlayerHealthComponent.h"

using namespace Engine;

GameLoop::GameLoop(Engine::GameObject* pOwner, ScoreComponent* pScoreComponent) :
	Component(pOwner),
	m_pScoreComponent{ pScoreComponent }
{
	m_pScoreComponent->GetOnScoreChange()->AddObserver(this);
}

void GameLoop::Start()
{
	CreateStartText();
}

void GameLoop::BeginGame()
{
	m_GameState = GameState::Running;

	auto pScene{ SceneManager::GetInstance().GetActiveScene() };

	m_pMapObject = PrefabFactory::Map1Parent(pScene);
	m_pMapObject->GetTransform()->SetWorldLocation(0, 72);

	//Spawn Player
	if (m_pPlayerObject == nullptr)
	{
		m_pPlayerObject = PrefabFactory::AddPlayer(pScene);
		m_pPlayerObject->GetTransform()->SetWorldLocation(128, 188);
		m_pPlayerHealthComponent = m_pPlayerObject->GetComponent<PlayerHealthComponent>();
		m_pPlayerHealthComponent->GetOnTakeDamage()->AddObserver(this);
	}
	else
	{
		m_pPlayerObject->GetTransform()->SetWorldLocation(128, 188);
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
