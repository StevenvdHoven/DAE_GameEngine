#pragma once
#include <vec2.hpp>
#include <string>

struct SDL_Texture;

/**
 * Simple RAII wrapper for an SDL_Texture
 */

namespace Engine
{
	struct Vector2;
	class Texture2D final
	{
	public:
		SDL_Texture* GetSDLTexture() const;
		explicit Texture2D(SDL_Texture* texture);
		explicit Texture2D(const std::string& fullPath);
		~Texture2D();

		glm::ivec2 GetSize() const;
		void GetSize(Vector2& out_size) const;

		Texture2D(const Texture2D&) = delete;
		Texture2D(Texture2D&&) = delete;
		Texture2D& operator= (const Texture2D&) = delete;
		Texture2D& operator= (const Texture2D&&) = delete;
	private:
		SDL_Texture* m_texture;
	};
}

