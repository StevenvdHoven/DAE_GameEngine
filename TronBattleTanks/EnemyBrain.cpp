#include "EnemyBrain.h"
#include "EngineTime.h"
#include "GameObject.h"
#include "EnemyMovement.h"
#include "GameLoop.h"
#include "PrefabFactory.h"
#include "ServiceLocator.h"
#include "SceneManager.h"
#include "Collider.h"
#include "Renderer.h"
#include "PlayerHealthComponent.h"

using namespace Engine;


EnemyBrain::EnemyBrain(Engine::GameObject* pOwner,EnemyType enemyType,Engine::Graph* pGraph, float pathUpdateTime, float shootRate, GameLoop* const pGame, std::unique_ptr<EnemyShootCommand>&& shootCommand) :
	Component{ pOwner },
	m_EnemyType{ enemyType },
	m_PathUpdateRate{ pathUpdateTime },
	m_PathUpdateTimer{ pathUpdateTime },
	m_pTargetPlayer{ nullptr },
	m_pEnemyMovement{ nullptr },
	m_pGame{ pGame },
	m_pGraph{pGraph},
	m_ShootRate{shootRate},
	m_ShootTimer{ shootRate },
	m_ShootCommand{ std::move(shootCommand) }
{
}

void EnemyBrain::Start()
{
	m_pEnemyMovement = GetGameObject()->GetComponent<EnemyMovement>();
	m_pEnemyMovement->SetGraph(m_pGraph);
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

		if(m_EnemyType == EnemyType::TANK)
			CheckForShoot();
	}
}

void EnemyBrain::OnCollisionEnter(Engine::GameObject* other)
{
  	if (m_EnemyType == EnemyType::TANK) return;

	if (other == m_pTargetPlayer)
	{
		auto healthComponent{ other->GetComponent<PlayerHealthComponent>() };
		if (healthComponent)
		{
			healthComponent->TakeDamage(1);
			m_pGame->OnNotify(this);
		}
	}
}

//void EnemyBrain::Render() const
//{
//	const Engine::Vector2 origin{ GetGameObject()->GetTransform()->GetWorldLocation() };
//	const Engine::Vector2 forward{ GetGameObject()->GetTransform()->GetForward() };
//	
//	Renderer::GetInstance().SetColor({ 255,0,0,255 });
//	Renderer::GetInstance().RenderLine(origin, origin + forward * 1000);
//
//	if (m_TestHit)
//	{
//		Renderer::GetInstance().SetColor({ 255,0,255,255 });
//		Renderer::GetInstance().RenderLine(origin, m_TestHit->GetTransform()->GetWorldLocation());
//	}
//}

std::string EnemyBrain::GetTypeName() const
{
	return "EnemyBrain";
}

void EnemyBrain::CheckForShoot()
{
	if (m_ShootTimer <= m_ShootRate)
	{
		m_ShootTimer += Time::GetInstance().GetDeltaTime();
		return;
	}

	const Engine::Vector2 origin{ GetGameObject()->GetTransform()->GetWorldLocation() };
	const Engine::Vector2 forward{ GetGameObject()->GetTransform()->GetForward() };

	const std::vector<LayerMask> mask{ LayerMask::Player, LayerMask::Wall };
	const Collider* selfCollider{ GetGameObject()->GetComponent<Collider>() };
	Collider* outCollider{ nullptr };
	if (ServiceLocator::GetPhysicsSystem().RayCast(origin, forward, selfCollider, outCollider, mask))
	{
		if (outCollider == nullptr) return;
		
		auto owningObject{ outCollider->GetGameObject() };
		m_TestHit = owningObject;
		if (owningObject == m_pTargetPlayer)
		{
			m_ShootCommand->Execute();
			m_ShootTimer = 0.f;
		}
	}
}
