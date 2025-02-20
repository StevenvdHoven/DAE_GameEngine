#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"

namespace fs = std::filesystem;

void Engine::ResourceManager::Init(const std::filesystem::path& dataPath)
{
	m_dataPath = dataPath;

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

Texture2D* Engine::ResourceManager::LoadTexture(const std::string& file)
{
	const auto fullPath = m_dataPath/file;
	const auto filename = fs::path(fullPath).filename().string();
	if(m_loadedTextures.find(filename) == m_loadedTextures.end())
		m_loadedTextures.insert(std::pair(filename,std::make_unique<Texture2D>(fullPath.string())));
	return m_loadedTextures.at(filename).get();
}

Font* Engine::ResourceManager::LoadFont(const std::string& file, uint8_t size)
{
	const auto fullPath = m_dataPath/file;
	const auto filename = fs::path(fullPath).filename().string();
	const auto key = std::pair<std::string, uint8_t>(filename, size);
	if(m_loadedFonts.find(key) == m_loadedFonts.end())
		m_loadedFonts.insert(std::pair(key,std::make_unique<Font>(fullPath.string(), size)));
	return m_loadedFonts.at(key).get();
}