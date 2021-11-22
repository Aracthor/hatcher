#pragma once

#include "gl_types.hpp"

#include "hatcher/basic_types.hpp"

namespace hatcher
{
class VertexBufferObject;

class VertexArrayObject
{
public:
    VertexArrayObject();
    ~VertexArrayObject();

    void AttribVBO(const VertexBufferObject& parVBO, GLuint parLayoutLocation);

    void DrawArrays() const;
    void DrawElements(int count) const;

private:
    GLuint m_id;
    uint m_elementCount = 0;
};

} // namespace hatcher
