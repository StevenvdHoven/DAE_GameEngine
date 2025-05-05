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

	switch (m_ImageAllignment)
	{
	case Engine::ImageAllignment::TopRight:
		pos.x -= size.x;
		break;
	case Engine::ImageAllignment::BottomLeft:
		pos.y -= size.y;
		break;
	case Engine::ImageAllignment::BottomRight:
		pos.x -= size.x;
		pos.y -= size.y;
		break;
	case Engine::ImageAllignment::Centre:
		pos.x -= size.x / 2;
		pos.y -= size.y / 2;
		break;
	}

	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y, rot);
}
