#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/unique_ptr.hpp"

#include "BasicTextDrawer.hpp"
#include "WorldComponents/Score.hpp"

#include <sstream>

using namespace hatcher;

namespace
{

class ScoreRenderUpdater final : public RenderUpdater
{
public:
    ScoreRenderUpdater(const IRendering* rendering)
        : m_textDrawer(rendering->GetMaterialFactory().get())
    {
    }

    void Update(IApplication* application, const ComponentManager* componentManager,
                ComponentManager* renderComponentManager, IFrameRenderer& frameRenderer) override
    {
        const glm::vec2 textSize = {16, 16};
        const glm::vec2 textPosition = {20.f, frameRenderer.Resolution().y - textSize.y - 20.f};
        std::ostringstream oss;
        const Score* score = componentManager->ReadWorldComponent<Score>();
        oss << "Score: " << score->points;
        m_textDrawer.AddTextToDraw(oss.str().c_str(), textPosition, textSize);
        m_textDrawer.Draw(frameRenderer);
    }

    BasicTextDrawer m_textDrawer;
};

RenderUpdaterRegisterer<ScoreRenderUpdater> registerer;

} // namespace
