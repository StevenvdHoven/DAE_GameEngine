#include "ImageRenderer.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Transform.h"
#include "GameObject.h"

using namespace Engine;

Engine::ImageRenderer::ImageRenderer(GameObject* pOwner,const std::string& imagePath):
	Component{pOwner},
	m_pTexture{ ResourceManager::GetInstance().LoadTexture(imagePath)},
	m_ImageAllignment{ ImageAllignment::TopLeft }
{
	
}

void Engine::ImageRenderer::Update()
{
}

void Engine::ImageRenderer::Render() const
{
	const Transform* transform{ GetGameObject()->GetTransform()};

	auto pos{ transform->GetWorldLocation() };
	auto rot{ transform->GetWorldRotation() };

	Vector2 size{ 0,0 };
	m_pTexture->GetSize(size);

	SDL_Point pivot{ 0, 0 };

	switch (m_ImageAllignment)
	{
	case Engine::ImageAllignment::TopRight:
		pos.x -= size.x;
		pivot.x = static_cast<int>(size.x);
		pivot.y = 0;
		break;
	case Engine::ImageAllignment::BottomLeft:
		pos.y -= size.y;
		pivot.x = 0;
		pivot.y = static_cast<int>(size.y);
		break;
	case Engine::ImageAllignment::BottomRight:
		pos.x -= size.x;
		pos.y -= size.y;
		pivot.x = static_cast<int>(size.x);
		pivot.y = static_cast<int>(size.y);
		break;
	case Engine::ImageAllignment::Centre:
		pos.x -= size.x / 2;
		pos.y -= size.y / 2;
		pivot.x = static_cast<int>(size.x / 2);
		pivot.y = static_cast<int>(size.y / 2);
		break;
	case Engine::ImageAllignment::Custom:
		pos.x -= m_Pivot.x;
		pos.y -= m_Pivot.y;
		pivot.x = 0;
		pivot.y = 0;
		break;
	}

	pivot.x += static_cast<int>(m_Pivot.x);
	pivot.y += static_cast<int>(m_Pivot.y);

	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y, rot,&pivot);
	/*Renderer::GetInstance().SetColor(SDL_Color{ 0, 0, 255, 255 });
	Renderer::GetInstance().RenderRectangle(pos, size.x, size.y);*/
}
