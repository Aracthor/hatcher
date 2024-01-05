#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Graphics/Material.hpp"
#include "hatcher/Graphics/MaterialFactory.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/unique_ptr.hpp"

#include "BasicTextDrawer.hpp"
#include "WorldComponents/Lives.hpp"

using namespace hatcher;

namespace
{

class LivesRenderUpdater final : public RenderUpdater
{
public:
    LivesRenderUpdater(const IRendering* rendering)
        : m_textDrawer(rendering->GetMaterialFactory().get())
    {
        m_material =
            rendering->GetMaterialFactory()->CreateMaterial("shaders/hello_world.vert", "shaders/hello_world.frag");

        m_lifeMesh = make_unique<Mesh>(m_material.get(), Primitive::Lines);

        float positions[] = {
            0.f,   18.f,

            12.f,  -15.f,

            -12.f, -15.f,

            8.f,   -7.5f,

            -8.f,  -7.5f,
        };
        ushort indices[] = {0, 1, 0, 2, 3, 4};

        m_lifeMesh->Set2DPositions(positions, std::size(positions));
        m_lifeMesh->SetIndices(indices, std::size(indices));
    }

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
        const Lives* lives = componentManager->ReadWorldComponent<Lives>();
        glm::vec2 meshPosition = {30.f, frameRenderer.Resolution().y - 60.f};
        for (int i = 0; i < lives->remaining; i++)
        {
            frameRenderer.AddMeshToRender(m_lifeMesh.get(),
                                          glm::translate(glm::vec3(meshPosition.x, meshPosition.y, 0.f)));
            meshPosition.x += 30.f;
        }

        if (lives->remaining == 0)
        {
            m_textDrawer.AddTextToDraw("GAME OVER", glm::vec2(175.f, 275.f), glm::vec2(50.f, 50.f));
            m_textDrawer.Draw(frameRenderer);
        }
    }

    BasicTextDrawer m_textDrawer;
    unique_ptr<Material> m_material;
    unique_ptr<Mesh> m_lifeMesh;
};

RenderUpdaterRegisterer<LivesRenderUpdater> registerer;

} // namespace
