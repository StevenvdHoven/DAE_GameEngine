#include "Achievements.h"
#include "GameObject.h"
#include "ScoreComponent.h"

using namespace Engine;

WinGameAchievement::WinGameAchievement(int id, GameObject* pPlayer) :
	Achievement{ id },
	m_pPlayer{ pPlayer }
{
	m_pScoreComponent = pPlayer->GetComponent<ScoreComponent>();
	m_pScoreComponent->GetOnScoreChange()->AddObserver(this);
}

void WinGameAchievement::OnNotify(Engine::Component* sender)
{
	if (sender != nullptr) return;

	bool m_bAchieved = false;
	//SteamUserStats()->GetAchievement("ACH_WIN_ONE_GAME", &m_bAchieved);
	if (m_bAchieved)
	{
		return;
	}

	if (m_pScoreComponent->GetScore() >= 1000)
	{
		/*SteamUserStats()->SetAchievement("ACH_WIN_ONE_GAME");
		SteamUserStats()->StoreStats();*/

		m_pScoreComponent->GetOnScoreChange()->RemoveObserver(this);
	}

}
