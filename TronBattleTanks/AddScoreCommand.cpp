#include "AddScoreCommand.h"
#include "GameObject.h"
#include "ScoreComponent.h"



AddScoreCommand::AddScoreCommand(GameObject* pGameObject) :
	Engine::GameActorCommand{ pGameObject },
	m_pScoreComponent{ nullptr }
{

}
void AddScoreCommand::Execute()
{
	if (m_pScoreComponent == nullptr)
		m_pScoreComponent = GetActor()->GetComponent<ScoreComponent>();

	if (m_pScoreComponent != nullptr)
		m_pScoreComponent->AddScore(100);
}
