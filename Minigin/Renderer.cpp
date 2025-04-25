#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "imgui.h"
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include "ServiceLocator.h"
#include "PhysicsSystem.h"


int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void Engine::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();
}

void Engine::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();

	ServiceLocator::GetPhysicsSystem().Render();



	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
	SDL_RenderPresent(m_renderer);
}

void Engine::Renderer::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void Engine::Renderer::RenderRectangle(const Vector2& pos, float width, float height) const
{
	SDL_Rect rect{};
	rect.x = static_cast<int>(pos.x);
	rect.y = static_cast<int>(pos.y);
	rect.w = static_cast<int>(width);
	rect.h = static_cast<int>(height);
	SDL_RenderDrawRect(m_renderer, &rect);
}

void Engine::Renderer::RenderFilledRectangle(const Vector2& pos, float width, float height) const
{
	SDL_Rect rect{};
	rect.x = static_cast<int>(pos.x);
	rect.y = static_cast<int>(pos.y);
	rect.w = static_cast<int>(width);
	rect.h = static_cast<int>(height);
	SDL_RenderFillRect(m_renderer, &rect);
}

void Engine::Renderer::RenderOval(const Vector2& pos, float radiusX, float RadiusY) const
{
	SDL_Rect rect{};
	rect.x = static_cast<int>(pos.x - radiusX);
	rect.y = static_cast<int>(pos.y - RadiusY);
	rect.w = static_cast<int>(radiusX * 2);
	rect.h = static_cast<int>(RadiusY * 2);
	// Draw the outline of the oval
	for (int w = 0; w < rect.w; w++)
	{
		for (int h = 0; h < rect.h; h++)
		{
			int dx = rect.w / 2 - w;
			int dy = rect.h / 2 - h;
			if ((dx * dx) * (rect.h * rect.h) + (dy * dy) * (rect.w * rect.w) <= (rect.w * rect.h) * (rect.w * rect.h))
			{
				SDL_RenderDrawPoint(m_renderer, rect.x + w, rect.y + h);
			}
		}
	}
}

void Engine::Renderer::RenderFilledOval(const Vector2& pos, float radiusX, float RadiusY) const
{
	SDL_Rect rect{};
	rect.x = static_cast<int>(pos.x - radiusX);
	rect.y = static_cast<int>(pos.y - RadiusY);
	rect.w = static_cast<int>(radiusX * 2);
	rect.h = static_cast<int>(RadiusY * 2);
	// Draw the filled oval
	for (int w = 0; w < rect.w; w++)
	{
		for (int h = 0; h < rect.h; h++)
		{
			int dx = rect.w / 2 - w;
			int dy = rect.h / 2 - h;
			if ((dx * dx) * (rect.h * rect.h) + (dy * dy) * (rect.w * rect.w) <= (rect.w * rect.h) * (rect.w * rect.h))
			{
				SDL_RenderDrawPoint(m_renderer, rect.x + w, rect.y + h);
			}
		}
	}

}

void Engine::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void Engine::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);

}

void Engine::Renderer::RenderTexture(const Texture2D& texture, float x, float y, float angle) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst, angle, nullptr, SDL_FLIP_NONE);
}

void Engine::Renderer::RenderTexture(const Texture2D& texture, float x, float y, float width, float height, float angle) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);

	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst, angle, nullptr, SDL_FLIP_NONE);
}

SDL_Renderer* Engine::Renderer::GetSDLRenderer() const { return m_renderer; }
