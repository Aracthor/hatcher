#pragma once

#include "hatcher/Maths/Box.hpp"
#include "hatcher/Maths/Mat.hpp"
#include "hatcher/Maths/Vec.hpp"

namespace hatcher
{

class Clock;
class Material;

class IFrameRenderer
{
public:
    virtual ~IFrameRenderer() = default;

    virtual void PrepareSceneDraw(const Material* material) const = 0;
    virtual void PrepareUIDraw(const Material* material) const = 0;
    virtual void SetProjectionMatrix(const Mat4f& matrix) = 0;
    virtual void SetViewMatrix(const Mat4f& matrix) = 0;
    virtual void EnableDepthTest() = 0;
    virtual void DisableDepthTest() = 0;

    virtual int CurrentTick() const = 0;
    virtual const Clock* GetClock() const = 0;
    virtual Vec2i Resolution() const = 0;
    virtual Vec2f WorldCoordsToWindowCoords(Vec3f worldCoords, const Mat4f& modelMatrix) const = 0;
    virtual Box2f ProjectBox3DToWindowCoords(const Box3f& box, const Mat4f& modelMatrix) const = 0;

    virtual Vec3f WindowCoordsToWorldCoords(Vec2f windowCoords) const = 0;
};

} // namespace hatcher
