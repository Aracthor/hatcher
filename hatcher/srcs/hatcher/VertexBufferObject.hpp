#pragma once

#include "basic_types.hpp"
#include "gl_types.hpp"

namespace hatcher
{

class VertexBufferObject
{
public:
    VertexBufferObject();
    ~VertexBufferObject();

    void SetData(float* data, uint length);

    void Bind() const;
    uint ElementCount() const { return m_elementCount; }

private:
    GLuint m_id;
    uint m_elementCount = 0;
};

} // namespace hatcher
