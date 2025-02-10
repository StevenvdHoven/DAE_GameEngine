#pragma once
#include <filesystem>
#include <string>
#include <memory>
#include <map>
#include "Singleton.h"
#include "Texture2D.h"
#include "Font.h"

class ResourceManager final : public Singleton<ResourceManager>
{
public:
	void Init(const std::filesystem::path& data);
	Texture2D* LoadTexture(const std::string& file);
	Font* LoadFont(const std::string& file, uint8_t size);
private:
	friend class Singleton<ResourceManager>;
	ResourceManager() = default;
	std::filesystem::path m_dataPath;

	//My opinion is that recoures can't be unloaded because the player might need them later again.
	//So instead of large image having to unload and reload, I rather just back it up in case.
	//void UnloadUnusedResources();

	std::map<std::string, std::unique_ptr<Texture2D>> m_loadedTextures;
	std::map<std::pair<std::string, uint8_t>, std::unique_ptr<Font>> m_loadedFonts;

};

