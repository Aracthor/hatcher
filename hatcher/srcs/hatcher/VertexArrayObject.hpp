#pragma once

#include <GL/glew.h>

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
    unsigned int m_elementCount = 0;
};

} // namespace hatcher