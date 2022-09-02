#pragma once

namespace hatcher
{
class ComponentManager;
class IEventUpdater;
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

using CreateRenderUpdaterFunction = RenderUpdater*(const IRendering* rendering,
                                                   IEventUpdater* eventUpdater);
template <class UpdaterClass>
int RegisterRenderUpdater(const char* name)
{
    int RegisterRenderUpdater(const char* name, CreateRenderUpdaterFunction* createFunction);
    return RegisterRenderUpdater(
        name, [](const IRendering* rendering, IEventUpdater* eventUpdater) -> RenderUpdater* {
            return new UpdaterClass(rendering, eventUpdater);
        });
}

} // namespace hatcher
