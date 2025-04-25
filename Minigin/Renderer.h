#pragma once
#include <SDL.h>
#include "Singleton.h"
#include "Texture2D.h"
#include "Vector2.h"
/**
 * Simple RAII wrapper for the SDL renderer
 */
namespace Engine
{
	struct Color
	{
		Uint8 r;
		Uint8 g;
		Uint8 b;
		Uint8 a;

		Color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 255)
			: r(red), g(green), b(blue), a(alpha) {}

		SDL_Color ToSDLColor() const
		{
			return SDL_Color{ r, g, b, a };
		}
	};

	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderRectangle(const Vector2& pos, float width, float height) const;
		void RenderFilledRectangle(const Vector2& pos, float width, float height) const;

		void RenderOval(const Vector2& pos, float radiusX, float RadiusY) const;
		void RenderFilledOval(const Vector2& pos, float radiusX, float RadiusY) const;

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;

		void RenderTexture(const Texture2D& texture, float x, float y, float angle) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height, float angle) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }

		void SetColor(const Color* color) const
		{
			SetColor(color->ToSDLColor());
		}

		void SetColor(const SDL_Color& color) const
		{
			SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
		}
	};
}

