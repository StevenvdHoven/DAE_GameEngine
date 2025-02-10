#include "ImageRenderer.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Transform.h"

ImageRenderer::ImageRenderer(const std::string& imagePath):
	m_pTexture{ ResourceManager::GetInstance().LoadTexture(imagePath)}
{
	
}

void ImageRenderer::Update()
{
}

void ImageRenderer::Render() const
{
	const Transform* transform{ GetTransform() };
	Renderer::GetInstance().RenderTexture(*m_pTexture, transform->GetPosition().x, transform->GetPosition().y);
}
