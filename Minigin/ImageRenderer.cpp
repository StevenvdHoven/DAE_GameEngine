#include "ImageRenderer.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Transform.h"

ImageRenderer::ImageRenderer(GameObject* pOwner,const std::string& imagePath):
	Component{pOwner},
	m_pTexture{ ResourceManager::GetInstance().LoadTexture(imagePath)}
{
	
}

void ImageRenderer::Update()
{
}

void ImageRenderer::Render() const
{
	const Transform* transform{ GetTransform() };

	auto pos{ transform->GetWorldLocation() };

	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}
