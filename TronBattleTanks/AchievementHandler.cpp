#include "AchievementHandler.h"
#include "Achievements.h"

AchievementHandler::AchievementHandler()
{
}

AchievementHandler::~AchievementHandler()
{
}

void AchievementHandler::AddAchievement(std::unique_ptr<Achievement> pAchievement)
{
	m_Achievements.emplace_back(std::move(pAchievement));
}

void AchievementHandler::RemoveAchievement(int id)
{
	auto it = std::find_if(m_Achievements.begin(), m_Achievements.end(), [id](const std::unique_ptr<Achievement>& pAchievement)
		{
			return pAchievement->GetID() == id;
		});

	if (it != m_Achievements.end())
	{
		m_Achievements.erase(it);
	}
}

void AchievementHandler::ValidateAchievements()
{
}
