#pragma once
#include "Component.h"

#define SCOREBOARD_FILE_PATH "ScoreBoard.json"
#define SCOREBOARD_MAX_ENTRIES 5

namespace Engine
{
	class TextRenderer;
}

struct ScoreEntry
{
	std::string Name;
	int Score;

	ScoreEntry() = default;
	ScoreEntry(const std::string& name, int score)
		: Name(name), Score(score) {}

	nlohmann::json Serialize() const
	{
		nlohmann::json json;
		json["score_entry_name"] = Name;
		json["score_entry_score"] = Score;
		return json;
	}

};

class ScoreBoardReaderWriter final
{
public:
	static std::vector<ScoreEntry> ReadScoreBoard(const std::string& filePath);
	static void WriteScoreBoard(const std::string& filePath, const std::vector<ScoreEntry>& scoreEntries);
};

class Scoreboard : public Engine::Component
{
public:
	Scoreboard(Engine::GameObject* pOwner);
	virtual ~Scoreboard() = default;

	void Start() override;
	void Refresh();

	std::string GetTypeName() const override;

private:
	void LoadTextComponents();
	void LoadScoreBoard();

	std::vector<Engine::TextRenderer*> m_pTextComponents;
};

