#include <SDL.h>
#include <SDL_image.h>
#include "Texture2D.h"
#include "Renderer.h"
#include <stdexcept>

using namespace Engine;

Engine::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_texture);
}

glm::ivec2 Engine::Texture2D::GetSize() const
{
	SDL_Rect dst;
	SDL_QueryTexture(GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	return { dst.w,dst.h };
}

void Engine::Texture2D::GetSize(Vector2& out_size) const
{
	SDL_Rect dst;
	SDL_QueryTexture(GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	out_size.x = static_cast<float>(dst.w);
	out_size.y = static_cast<float>(dst.h);
}

SDL_Texture* Engine::Texture2D::GetSDLTexture() const
{
	return m_texture;
}

Engine::Texture2D::Texture2D(const std::string &fullPath)
{
	m_texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
}

Engine::Texture2D::Texture2D(SDL_Texture* texture)	: m_texture{ texture }
{
	assert(m_texture != nullptr);
}

