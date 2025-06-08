#include "Scoreboard.h"
#include "GameObject.h"
#include "Transform.h"
#include "TextRenderer.h"
#include <fstream>
#include <filesystem>



Scoreboard::Scoreboard(Engine::GameObject* pOwner):
	Engine::Component{ pOwner }
{
}

void Scoreboard::Start()
{
	LoadTextComponents();
	LoadScoreBoard();
}

void Scoreboard::Refresh()
{
	LoadScoreBoard();
}

std::string Scoreboard::GetTypeName() const
{
	return "Scoreboard";
}

void Scoreboard::LoadTextComponents()
{
	m_pTextComponents.resize(5);
	for (int index{ 0 }; index < m_pTextComponents.size(); ++index)
	{
		auto textName{ std::to_string(index) + "-RANKTEXT" };
		m_pTextComponents[index] = GetGameObject()->GetTransform()->FindObjectByNameInChilderen(textName)->GetComponent<Engine::TextRenderer>();
	}
}

void Scoreboard::LoadScoreBoard()
{
	for (auto& textComp : m_pTextComponents)
	{
		textComp->IsEnabled = false;
	}

	auto scoreEntries = ScoreBoardReaderWriter::ReadScoreBoard(SCOREBOARD_FILE_PATH);
	if (scoreEntries.empty())
	{
		return;
	}

	for (size_t i = 0; i < m_pTextComponents.size() && i < scoreEntries.size(); ++i)
	{
		m_pTextComponents[i]->IsEnabled = true;
		m_pTextComponents[i]->SetText(std::to_string(i + 1) + " " + scoreEntries[i].Name + " " + std::to_string(scoreEntries[i].Score));
	}
}

std::vector<ScoreEntry> ScoreBoardReaderWriter::ReadScoreBoard(const std::string& filePath)
{
	std::vector<ScoreEntry> scoreEntries;

	if (!std::filesystem::exists(filePath))
	{
		return scoreEntries;
	}

	std::ifstream file(filePath);
	if (!file.is_open())
	{
		return scoreEntries;
	}

	nlohmann::json jsonData;
	file >> jsonData;

	for (const auto& entry : jsonData)
	{
		ScoreEntry temp{ entry["score_entry_name"].get<std::string>(), entry["score_entry_score"].get<int>()};
		scoreEntries.emplace_back(temp);
	}

	return scoreEntries;
}

void ScoreBoardReaderWriter::WriteScoreBoard(const std::string& filePath, const std::vector<ScoreEntry>& scoreEntries)
{
	nlohmann::json jsonData;
	for (const auto& entry : scoreEntries)
	{
		jsonData.emplace_back(entry.Serialize());
	}

	std::ofstream file(filePath);
	if (file.is_open())
	{
		file << jsonData.dump(4);
	}
}
