#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"

using namespace hatcher;

namespace
{

class ProjectionViewUpdater final : public RenderUpdater
{
public:
    ProjectionViewUpdater(const IRendering* rendering) {}

    void Update(IApplication* application, const ComponentAccessor* componentAccessor,
                ComponentAccessor* renderComponentAccessor, IFrameRenderer& frameRenderer) override
    {
        const Vect2i resolution = frameRenderer.Resolution();
        frameRenderer.SetProjectionMatrix(Mat4f::Orthographic(0.f, float(resolution.x), 0.f, float(resolution.y)));
    }
};

RenderUpdaterRegisterer<ProjectionViewUpdater> registerer(-1);

} // namespace
