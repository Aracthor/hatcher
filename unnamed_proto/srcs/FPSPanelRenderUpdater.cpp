#include "hatcher/Graphics/Clock.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"

#include "imgui.h"

using namespace hatcher;

namespace
{

class FPSPanelRenderUpdater final : public RenderUpdater
{
public:
    FPSPanelRenderUpdater(const IRendering* rendering) {}

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
        const float elapsedTime = frameRenderer.GetClock()->GetElapsedTime();
        m_totalElapsedTime += elapsedTime;
        m_frameTimes.push_back(elapsedTime);
        if (m_totalElapsedTime >= 1000.f)
            ProcessSecondTimes();

        ImGui::SetNextWindowSize({400, 100}, ImGuiCond_Always);
        ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Always);
        if (ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground))
        {
            ImGui::Text("Frame time: %2.2f (%2.2f..%2.2f)\n", m_averageFrame, m_shortestFrame, m_longestFrame);
            ImGui::Text("FPS: %2.0f\n", m_fps);
        }
        ImGui::End();
    }

private:
    void ProcessSecondTimes()
    {
        m_shortestFrame = std::numeric_limits<float>::max();
        m_averageFrame = 0.f;
        m_longestFrame = 0.f;
        for (float frameTime : m_frameTimes)
        {
            m_shortestFrame = std::min(m_shortestFrame, frameTime);
            m_averageFrame += frameTime;
            m_longestFrame = std::max(m_longestFrame, frameTime);
        }
        m_averageFrame /= (float)m_frameTimes.size();
        m_fps = m_totalElapsedTime / m_averageFrame;

        m_totalElapsedTime = 0.f;
        m_frameTimes.clear();
    }

    float m_totalElapsedTime = 0.f;
    std::vector<float> m_frameTimes;

    float m_shortestFrame = 0.f;
    float m_averageFrame = 0.f;
    float m_longestFrame = 0.f;
    float m_fps = 0.f;
};

RenderUpdaterRegisterer<FPSPanelRenderUpdater> registerer;

} // namespace
