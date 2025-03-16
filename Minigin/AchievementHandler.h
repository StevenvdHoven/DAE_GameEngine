#pragma once
#include "Singleton.h"
#include <memory>
#include <vector>

class Achievement;
class AchievementHandler final : public Singleton<AchievementHandler>
{
public:
	AchievementHandler();
	~AchievementHandler();

	void AddAchievement(std::unique_ptr<Achievement> pAchievement);
	void RemoveAchievement(int id);

	void ValidateAchievements();

private:
	std::vector<std::unique_ptr<Achievement>> m_Achievements;

};


