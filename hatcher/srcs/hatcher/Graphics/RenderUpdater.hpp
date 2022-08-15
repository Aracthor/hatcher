#pragma once

namespace hatcher
{
class ComponentManager;
class IFrameRenderer;
class IRendering;

class RenderUpdater
{
public:
    virtual ~RenderUpdater() = default;

    virtual void Update(const ComponentManager* componentManager,
                        ComponentManager* renderComponentManager,
                        IFrameRenderer& frameRenderer) = 0;
};

using CreateRenderUpdaterFunction = RenderUpdater*(const IRendering* rendering);
template <class UpdaterClass>
int RegisterRenderUpdater(const char* name)
{
    int RegisterRenderUpdater(const char* name, CreateRenderUpdaterFunction* createFunction);
    return RegisterRenderUpdater(name, [](const IRendering* rendering) -> RenderUpdater* {
        return new UpdaterClass(rendering);
    });
}

} // namespace hatcher
