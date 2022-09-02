#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/assert.hpp"

#include "imgui.h"

using namespace hatcher;

namespace
{

class DemoImguiRenderUpdater final : public RenderUpdater
{
public:
    DemoImguiRenderUpdater(const IRendering* rendering, IEventUpdater* eventUpdater) {}

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
        ImGui::ShowDemoWindow();
    }
};

const int dummy = RegisterRenderUpdater<DemoImguiRenderUpdater>("DemoImgui");

} // namespace