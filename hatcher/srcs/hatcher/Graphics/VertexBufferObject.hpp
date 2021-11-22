#pragma once

#include "gl_types.hpp"

#include "hatcher/basic_types.hpp"

namespace hatcher
{

class VertexBufferObject
{
public:
    VertexBufferObject();
    ~VertexBufferObject();

    void SetData(float* data, uint length);
    void SetData(ushort* data, uint length);

    void Bind() const;
    uint ElementCount() const { return m_elementCount; }

private:
    GLuint m_id;
    GLenum m_target = 0;
    uint m_elementCount = 0;
};

} // namespace hatcher
