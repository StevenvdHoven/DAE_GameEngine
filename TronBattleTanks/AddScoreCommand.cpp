#include "AddScoreCommand.h"
#include "GameObject.h"
#include "ScoreComponent.h"
#include "ServiceLocator.h"



AddScoreCommand::AddScoreCommand(GameObject* pGameObject) :
	Engine::GameActorCommand{ pGameObject },
	m_pScoreComponent{ nullptr },
	m_ScoreSound{ Engine::ServiceLocator::GetSoundSystem().LoadSound("pickupCoin.wav") }
{
	
}
void AddScoreCommand::Execute()
{
	if (m_pScoreComponent == nullptr)
		m_pScoreComponent = GetActor()->GetComponent<ScoreComponent>();

	if (m_pScoreComponent != nullptr)
	{
		m_pScoreComponent->AddScore(100);
		Engine::ServiceLocator::GetSoundSystem().PlaySound(m_ScoreSound);
	}
		
}
