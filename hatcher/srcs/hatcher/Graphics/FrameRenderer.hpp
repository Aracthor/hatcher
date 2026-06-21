#pragma once

#include "IFrameRenderer.hpp"

#include "hatcher/Maths/Mat.hpp"
#include "hatcher/Maths/Vect.hpp"

namespace hatcher
{
class GLContext;

class FrameRenderer : public IFrameRenderer
{
public:
    FrameRenderer(GLContext* context, const Clock* clock, Vect2i resolution);
    ~FrameRenderer();

    void SetCurrentTick(int currentTick) { m_currentTick = currentTick; }
    void PrepareSceneDraw(const Material* material) const override;
    void PrepareUIDraw(const Material* material) const override;
    void SetProjectionMatrix(const Mat4f& matrix) override;
    void SetViewMatrix(const Mat4f& matrix) override;
    void EnableDepthTest() override;
    void DisableDepthTest() override;

    int CurrentTick() const override { return m_currentTick; }
    const Clock* GetClock() const override { return m_clock; }
    Vect2i Resolution() const override { return m_resolution; }
    Vect2f WorldCoordsToWindowCoords(Vect3f worldCoords, const Mat4f& modelMatrix) const override;
    Box2f ProjectBox3DToWindowCoords(const Box3f& box, const Mat4f& modelMatrix) const override;
    Vect3f WindowCoordsToWorldCoords(Vect2f windowCoords) const override;

private:
    GLContext* m_context;
    int m_currentTick = 0;
    const Clock* m_clock;
    const Vect2i m_resolution;

    Mat4f m_viewMatrix = Mat4f::Identity();
    Mat4f m_projectionMatrix = Mat4f::Identity();
};

} // namespace hatcher
