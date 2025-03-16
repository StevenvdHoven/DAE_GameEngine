#include "AddScoreCommand.h"
#include "GameObject.h"
#include "ScoreComponent.h"


Engine::AddScoreCommand::AddScoreCommand(GameObject* pGameObject) :
	GameActorCommand{ pGameObject },
	m_pScoreComponent{ nullptr }
{

}
void Engine::AddScoreCommand::Execute()
{
	if (m_pScoreComponent == nullptr)
		m_pScoreComponent = GetActor()->GetComponent<ScoreComponent>();

	if (m_pScoreComponent != nullptr)
		m_pScoreComponent->AddScore(100);
}
