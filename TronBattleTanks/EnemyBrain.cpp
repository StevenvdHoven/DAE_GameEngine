#include "EnemyBrain.h"
#include "EngineTime.h"
#include "GameObject.h"
#include "EnemyMovement.h"
#include "GameLoop.h"

using namespace Engine;


EnemyBrain::EnemyBrain(Engine::GameObject* pOwner, float pathUpdateTime, GameLoop* pGame):
	Component{pOwner},
	m_PathUpdateRate{pathUpdateTime},
	m_PathUpdateTimer{0.f},
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
	m_PathUpdateTimer += Time::GetInstance().GetDeltaTime();
	if (m_PathUpdateTimer >= m_PathUpdateRate)
	{
		m_PathUpdateTimer = 0.f;
		m_pEnemyMovement->SetTargetPosition(Vector2::Zero());
	}
}
