#include "Animaton.h"
#include "GameObject.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "EngineTime.h"
#include "ImageRenderer.h"
#include "imgui.h"

using namespace Engine;

Engine::Animation::Animation(GameObject* pOwner):
    Component{pOwner},
    m_PlayAtStart{false},
    m_Playing{false},
    m_Loop{false},
    m_AnimationIndex{0},
    m_PlayRate{1.f},
    m_AnimationTimer{0.f},
    m_Renderer{nullptr},
    m_AnimationImagePaths{},
    m_AnimationImages{}
{
}

void Engine::Animation::Start()
{
    m_Renderer = GetGameObject()->GetComponent<ImageRenderer>();
    if (m_PlayAtStart)
    {
        Play();
    }
}

void Engine::Animation::Update()
{
    if (m_Playing)
    {
        if (!m_Renderer) m_Renderer = GetGameObject()->GetComponent<ImageRenderer>();
        m_AnimationTimer += Time::GetInstance().GetDeltaTime();
        if (m_AnimationTimer >= m_PlayRate)
        {
            m_AnimationTimer = 0;
            ++m_AnimationIndex;
            if (m_AnimationIndex >= m_AnimationImages.size())
            {
                if (m_Loop) m_AnimationIndex = 0;
                else
                {
                    m_Playing = false;
                    return;
                }
            }
            m_Renderer->SetImage(m_AnimationImages[m_AnimationIndex]);
        }
    }
}

void Engine::Animation::Play()
{
    if (m_Playing) return;
    if(!m_Renderer) m_Renderer = GetGameObject()->GetComponent<ImageRenderer>();


    m_Playing = true;
    m_AnimationIndex = 0;
    m_Renderer->SetImage(m_AnimationImages[0]);
}

void Engine::Animation::Stop()
{
    if (!m_Playing) return;

    m_Playing = false;
    m_Renderer->SetImage(m_AnimationImages[0]);
}

void Engine::Animation::SetPlayRate(float playRate)
{
    m_PlayRate = playRate;
}

void Engine::Animation::SetSequence(std::vector<std::string>&& filePath)
{
    m_AnimationImagePaths = std::move(filePath);
    m_AnimationImages.clear();
    m_AnimationImages.resize(m_AnimationImagePaths.size());

    for (int index{ 0 }; index < m_AnimationImages.size(); ++index)
    {
        m_AnimationImages[index] = ResourceManager::GetInstance().LoadTexture(m_AnimationImagePaths[index]);
    }
}

void Engine::Animation::GUI()
{
    if (ImGui::Button("Play"))
    {
        Play();
    }

    ImGui::SameLine();

    if (ImGui::Button("Stop"))
    {
        Stop();
    }

    ImGui::Checkbox("Play at Start", &m_PlayAtStart);
    ImGui::Checkbox("Loop", &m_Loop);

    ImGui::SliderFloat("Play Rate", &m_PlayRate, 0.01f, 10.0f, "%.2fx");

    if (!m_AnimationImages.empty())
    {
        ImGui::SliderInt("Current Frame", &m_AnimationIndex, 0, static_cast<int>(m_AnimationImages.size() - 1));
        if (m_Renderer && m_AnimationIndex >= 0 && m_AnimationIndex < static_cast<int>(m_AnimationImages.size()))
        {
            m_Renderer->SetImage(m_AnimationImages[m_AnimationIndex]);
        }
    }

    bool shouldReloadImages = false;
    if (ImGui::TreeNode("Animation Frames"))
    {
        for (size_t i = 0; i < m_AnimationImagePaths.size(); ++i)
        {
            char buffer[256];
            strncpy_s(buffer, m_AnimationImagePaths[i].c_str(), sizeof(buffer));
            buffer[sizeof(buffer) - 1] = '\0';

            if (ImGui::InputText(("Frame " + std::to_string(i)).c_str(), buffer, sizeof(buffer)))
            {
                m_AnimationImagePaths[i] = buffer;
                shouldReloadImages = true;
            }
        }

        if (ImGui::Button("Add Frame"))
        {
            m_AnimationImagePaths.emplace_back("path/to/image.png");
            shouldReloadImages = true;
        }

        if (!m_AnimationImagePaths.empty() && ImGui::Button("Remove Last Frame"))
        {
            m_AnimationImagePaths.pop_back();
            shouldReloadImages = true;
        }

        ImGui::TreePop();
    }

    if (shouldReloadImages)
    {
        m_AnimationImages.clear();
        for (const auto& path : m_AnimationImagePaths)
        {
            Texture2D* texture = ResourceManager::GetInstance().LoadTexture(path); // Replace this with your texture loading function
            m_AnimationImages.emplace_back(texture);
        }
    }
}


void Engine::Animation::Serialize(nlohmann::json& json) const
{
    json["animation_play_at_start"] = m_PlayAtStart;
    json["animation_loop"] = m_Loop;
    json["animation_play_rate"] = m_PlayRate;
    json["animation_image_paths"] = m_AnimationImagePaths;
}

void Engine::Animation::Deserialize(const nlohmann::json& json)
{
    m_PlayAtStart = json["animation_play_at_start"].get<bool>();
    m_Loop = json["animation_loop"].get<bool>();
    m_PlayRate = json["animation_play_rate"].get<float>();
    m_AnimationImagePaths = json["animation_image_paths"].get<std::vector<std::string>>();
    SetSequence(std::move(m_AnimationImagePaths));
}

std::string Animation::GetTypeName() const
{
    return "Animation";
}
