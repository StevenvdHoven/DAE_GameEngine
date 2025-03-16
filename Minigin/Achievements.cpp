#include "Achievements.h"
#include "GameObject.h"
#include "ScoreComponent.h"
#include "steam_api.h"


WinGameAchievement::WinGameAchievement(int id, GameObject* pPlayer) :
	Achievement{ id },
	m_pPlayer{ pPlayer }
{
	m_pScoreComponent = pPlayer->GetComponent<ScoreComponent>();
	m_pScoreComponent->GetOnScoreChange()->AddObserver(this);
}

void WinGameAchievement::OnNotify()
{
	bool m_bAchieved = false;
	SteamUserStats()->GetAchievement("ACH_WIN_ONE_GAME", &m_bAchieved);
	if (m_bAchieved)
	{
		return;
	}

	if (m_pScoreComponent->GetScore() >= 1000)
	{
			SteamUserStats()->SetAchievement("ACH_WIN_ONE_GAME");
			SteamUserStats()->StoreStats();
	}

}
