#include "GameLoop.h"
#include "ScoreComponent.h"
#include "TextRenderer.h"

using namespace Engine;

GameLoop::GameLoop(Engine::GameObject* pOwner, ScoreComponent* pScoreComponent, Engine::GameObject* pMapObject) :
	Component(pOwner),
	m_pMapObject{pMapObject},
	m_pScoreComponent{ pScoreComponent }
{
	m_pScoreComponent->GetOnScoreChange()->AddObserver(this);
}

void GameLoop::Start()
{

}

void GameLoop::OnNotify(Component* sender)
{
	if (m_pScoreComponent == sender)
	{
		HandleScoreInfo();
	}
}

void GameLoop::HandleScoreInfo()
{
	
}
