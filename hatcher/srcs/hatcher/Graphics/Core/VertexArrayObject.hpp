#pragma once

#include "gl_types.hpp"

#include "hatcher/Graphics/Primitive.hpp"
#include "hatcher/basic_types.hpp"

namespace hatcher
{
class VertexBufferObject;

class VertexArrayObject
{
public:
    VertexArrayObject(Primitive::Type primitive);
    ~VertexArrayObject();

    void Bind();
    void Unbind();

    void AttribVBO(const VertexBufferObject& parVBO, GLint componentCount, GLuint layoutLocation);

    void DrawArrays(int count) const;
    void DrawElements(int count) const;

private:
    bool IsBinded() const;

    GLuint m_id;
    GLenum m_mode;
};

} // namespace hatcher
