#pragma once

#include "basic_types.hpp"
#include "gl_types.hpp"

namespace hatcher
{
class VertexBufferObject;

class VertexArrayObject
{
public:
    VertexArrayObject();
    ~VertexArrayObject();

    void AttribVBO(const VertexBufferObject& parVBO, GLuint parLayoutLocation);

    void Draw() const;

private:
    GLuint m_id;
    uint m_elementCount = 0;
};

} // namespace hatcher
