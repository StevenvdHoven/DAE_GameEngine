#include "EnemyBrain.h"
#include "EngineTime.h"
#include "GameObject.h"
#include "EnemyMovement.h"
#include "GameLoop.h"

using namespace Engine;


EnemyBrain::EnemyBrain(Engine::GameObject* pOwner, float pathUpdateTime, GameLoop* pGame):
	Component{pOwner},
	m_PathUpdateRate{pathUpdateTime},
	m_PathUpdateTimer{pathUpdateTime},
	m_pTargetPlayer{nullptr},
	m_pEnemyMovement{nullptr},
	m_pGame{pGame}
{
}

void EnemyBrain::Start()
{
	m_pEnemyMovement = GetGameObject()->GetComponent<EnemyMovement>();
}

void EnemyBrain::Update()
{
	if (!m_pTargetPlayer) {
		m_pTargetPlayer = m_pGame->GetRandomPlayer();
	}
	else
	{
		m_PathUpdateTimer += Time::GetInstance().GetDeltaTime();
		if (m_PathUpdateTimer >= m_PathUpdateRate)
		{
			m_PathUpdateTimer = 0.f;
			m_pEnemyMovement->SetTargetPosition(m_pTargetPlayer->GetTransform()->GetWorldLocation());
		}
	}
}
